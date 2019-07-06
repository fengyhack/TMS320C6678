/****************
*
* DSP-side 
* http://fengyh.cn/
*
*****************/

#include <c6x.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ti/ndk/inc/netmain.h>
#include <ti/ndk/inc/_stack.h>
#include <ti/ndk/inc/tools/console.h>
#include <ti/ndk/inc/tools/servers.h>
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/IHeap.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <xdc/runtime/knl/Thread.h>
#include <xdc/cfg/global.h>
#include <ti/platform/platform.h>
#include <ti/platform/resource_mgr.h>
#include <ti/omp/omp.h>
//------------------------------------------------------------------------------

#define MAX_NUM_DESC_OTHER 1024
#define PORT 7
#define MAX_IMG_SIZE 33554432 //8192*4096
#define UNIT 1024
#define MAX_LOOP 10

typedef unsigned char byte;

typedef struct tag_packet_info
{
	int width;
	int height;
	int unit;
	int cycle;
	int residue;
}packet_info;

typedef struct tag_data_t
{
	int width;
	int height;
	byte* data;
}data_t;

#define MSG_DEFAULT 0
#define MSG_GET_READY 1
#define MSG_SRCIMG_RECEIVED 2
#define MSG_IMGPROC_FINISHED 3
#define MSG_SENDBACK_FINISHED 4

char msg_str[5][32] =
{
	"DEFAULT",
	"GET_READY",
	"SRCIMG_RECEIVED",
	"IMGPROC_FINISHED",
	"SENDBACK_FINISHED"
};

typedef struct tag_message_t
{
	int id;
	data_t img_hdr;
}message_t;

message_t msg_s={MSG_DEFAULT};

char *HostName = "C6678";
char *LocalIPAddr = "0.0.0.0"; /* Set to "0.0.0.0" for DHCP client option */
char *PCStaticIP = "192.168.2.101"; /* Static IP address for host PC */
char *EVMStaticIP = "192.168.2.100"; /*    "   IP     "   for EVM */
char *LocalIPMask = "255.255.254.0"; /* Mask for DHCP Server option */
char *GatewayIP = "192.168.2.101"; /* Not used when using DHCP */
char *DomainName = "c6678.net"; /* Not used when using DHCP */
char *DNSServer = "0.0.0.0"; /* Used when set to anything but zero */

HANDLE hCfg = NULL;
HANDLE hUdp = NULL;
Mailbox_Handle mbox_srcimg = NULL;
Mailbox_Handle mbox_result = NULL;
int network_started=0;
int task_counter=0;

extern int32_t res_mgr_init_qmss_global(uint32_t max_num_desc);
extern void LLD_init(void);
extern void EVM_init(void);

void master_main(void);
static void NetworkOpen();
static void NetworkClose();
static void NetworkIPAddr(IPN IPAddr, uint IfIdx, uint fAdd);

int Daemon_proc(SOCKET s, UINT32 unused);
void ImageProc(byte* data,int width, int height);

//------------------------------------------------------------------------------
void main(void)
{
	printf("\nMulticore Image Processing using OpenMP\n\n");
	mbox_srcimg = Mailbox_create(sizeof(message_t), 1, 0, 0);
	mbox_result = Mailbox_create(sizeof(message_t), 1, 0, 0);

	printf("Execute Task_create...\n");
	Task_create((ti_sysbios_knl_Task_FuncPtr)master_main, NULL, NULL);

	while(!network_started)
	{
		printf("Waiting for the start of the network...\n");
		TaskSleep(1);
	}

	while(1)
	{
		if(task_counter>MAX_LOOP) break;
		msg_s.id = MSG_GET_READY;
		Mailbox_pend(mbox_srcimg, &msg_s, BIOS_WAIT_FOREVER);
		printf("MSM: Recv message: %s\n",msg_str[msg_s.id]);
		ImageProc(msg_s.img_hdr.data,msg_s.img_hdr.width,msg_s.img_hdr.height);
		msg_s.id = MSG_IMGPROC_FINISHED;
		Mailbox_post(mbox_result, &msg_s, BIOS_WAIT_FOREVER);
		printf("MSM: Send message: %s\n",msg_str[msg_s.id]);
	}

	printf("Execute Mailbox_delete...\n");
	Mailbox_delete(&mbox_result);
	Mailbox_delete(&mbox_srcimg);

	//printf("main::SockClose()\n");
	printf("Execute CfgFree...\n");
	CfgFree(hCfg);
	printf("Execute NC_SystemClose...\n");
	NC_SystemClose();
	printf("______ Program ends here _______");
}

void LLD_init(void)
{
	QMSS_CFG_T qmss_cfg;
	if(platform_get_coreid() == 0)
	{
		qmss_cfg.master_core = 1;
	}
	else
	{
		return;
	}
	res_mgr_init_qmss_global(MAX_NUM_DESC_OTHER + MAX_NUM_DESC);
	qmss_cfg.max_num_desc = MAX_NUM_DESC;
	qmss_cfg.desc_size = MAX_DESC_SIZE;
	qmss_cfg.mem_region = Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED;
	res_mgr_init_qmss(&qmss_cfg);
}

void EVM_init(void)
{
	if(platform_get_coreid() != 0)
	{
		return;
	}

	platform_init_flags sFlags;
	platform_init_config sConfig;
	memset((void *)&sFlags, 0, sizeof(platform_init_flags));
	memset((void *)&sConfig, 0, sizeof(platform_init_config));
	sFlags.pll = 0; /* PLLs for clocking      */
	sFlags.ddr = 0; /* External memory         */
	sFlags.tcsl = 1; /* Time stamp counter     */
	sFlags.phy = 1; /* Ethernet             */
	sFlags.ecc = 0; /* Memory ECC             */
	sConfig.pllm = 0; /* Use libraries default clock divisor */
	platform_init(&sFlags, &sConfig);
}

void master_main(void)
{
	platform_uart_init();
	platform_uart_set_baudrate(115200);
	platform_write_configure(PLATFORM_WRITE_ALL);

	CPPI_CFG_T cppi_cfg;
	if(platform_get_coreid() == 0)
	{
		cppi_cfg.master_core = 1;
	}
	else
	{
		cppi_cfg.master_core = 0;
	}
	cppi_cfg.dma_num = Cppi_CpDma_PASS_CPDMA;
	cppi_cfg.num_tx_queues = NUM_PA_TX_QUEUES;
	cppi_cfg.num_rx_channels = NUM_PA_RX_CHANNELS;

	res_mgr_init_cppi(&cppi_cfg);
	res_mgr_init_pass();

	printf("Execute NC_SystemOpen...\n");
	NC_SystemOpen( NC_PRIORITY_LOW, NC_OPMODE_INTERRUPT);

	printf("Execute CfgNew...\n");
	hCfg = CfgNew();
	CfgAddEntry(hCfg, CFGTAG_SYSINFO, CFGITEM_DHCP_HOSTNAME, 0, strlen(HostName), (UINT8 *)HostName, 0);

	if(platform_get_switch_state(1))
	{
		printf("get_switch_state(1) # DHCP_IS_ON.\n");
		CfgFree(hCfg);
		NC_SystemClose();
		return;
	}

	/* Setup an IP address to this EVM */
	CI_IPNET NA;
	bzero(&NA, sizeof(NA));
	NA.IPAddr = inet_addr(EVMStaticIP);
	NA.IPMask = inet_addr(LocalIPMask);
	strcpy(NA.Domain, DomainName);
	CfgAddEntry(hCfg, CFGTAG_IPNET, 1, 0, sizeof(CI_IPNET), (UINT8 *)&NA, 0);

	/* Add the default gateway (back to user PC) */
	CI_ROUTE RT;
	bzero(&RT, sizeof(RT));
	RT.IPDestAddr = inet_addr(PCStaticIP);
	RT.IPDestMask = inet_addr(LocalIPMask);
	RT.IPGateAddr = inet_addr(GatewayIP);
	CfgAddEntry(hCfg, CFGTAG_ROUTE, 0, 0, sizeof(CI_ROUTE), (UINT8 *)&RT, 0);

	printf("EVM in StaticIP mode at %s\n", EVMStaticIP);
	printf("Set IP address of PC to %s\n", PCStaticIP);

	NC_NetStart(hCfg, NetworkOpen, NetworkClose, NetworkIPAddr);
}

static void NetworkOpen()
{
	printf("Execute NetworkOpen...\n");
	hUdp=DaemonNew(SOCK_DGRAM,0,PORT,Daemon_proc,OS_TASKPRINORM,OS_TASKSTKNORM,0,1);
	network_started=1;
}

static void NetworkClose()
{
	printf("Execute NetworkClose...\n");
	DaemonFree(hUdp);
}

static void NetworkIPAddr(IPN IPAddr, uint IfIdx, uint fAdd)
{
	IPN IPTmp = ntohl(IPAddr);
	printf("IP Addr(%d):%d.%d.%d.%d\n", IfIdx, (UINT8)(IPTmp >> 24),
			(UINT8)(IPTmp >> 16), (UINT8)(IPTmp >> 8), (UINT8)IPTmp);
}

int Daemon_proc(SOCKET s, UINT32 unused)
{
	++task_counter;
	printf("------------------- TASK execution %d -------------------\n",task_counter);

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

	struct sockaddr_in sa;
	int nr,nr_sum;
	HANDLE hBuf = NULL;
	byte* pBuf = NULL;
	char* str_reply = "HELLO";
	int sz_sa = sizeof(sa);
	int sz_reply=strlen(str_reply);

	packet_info packi;
	nr=recvncfrom(s,(void**)&pBuf,0,(PSA)&sa,&sz_sa,&hBuf);
	if(nr<0)
	{
		printf("ERROR: recvncfrom() # error(%d)\n",nr);
		return -1;
	}
	memcpy(&packi,pBuf,sizeof(packet_info));
	sendto(s, str_reply, sz_reply, 0, (PSA)&sa, sz_sa);
	recvncfree(hBuf);

	int width=packi.width;
	int height=packi.height;
	int unit=packi.unit;
	int cycle=packi.cycle;
	int residue=packi.residue;
	int sz_img=width*height;
	if(sz_img<=0||sz_img>=MAX_IMG_SIZE)
	{
		printf("ERROR: Invalid image data size (%d)\n",sz_img);
		return -1;
	}

	printf("ImageSize[Width(%d) * Height(%d)] = %d\n", width, height,sz_img);
	byte* data = (byte*)malloc(sz_img);
	if(data == NULL)
	{
		printf("ERROR: Failed to allocate memory!\n");
		return -1;
	}

	// Receive image data
	if(task_counter==1)
	{
		printf("#WARNING# Exception may occur in the first loop.\n");
	}
	nr_sum = 0;
	byte* ptr = data;
	while(1)
	{
		nr = recvncfrom(s, (void**)&pBuf, 0, (PSA)&sa, &sz_sa, &hBuf);
		if(nr > 0)
		{
			memcpy(ptr, pBuf, nr);
			ptr += nr;
			nr_sum += nr;
			sendto(s, str_reply, sz_reply, 0, (PSA)&sa, sz_sa);
			recvncfree(hBuf);
		}
		else
		{
			break;
		}
	}

	if(nr_sum != sz_img)
	{
		printf("ERROR: Data transfer is incomplete (%d/%d)!\n", nr_sum, sz_img);
		free(data);
		return -1;
	}

	msg_s.id = MSG_SRCIMG_RECEIVED;
	msg_s.img_hdr.width=width;
	msg_s.img_hdr.height=height;
	msg_s.img_hdr.data=data;
	Mailbox_post(mbox_srcimg, &msg_s, BIOS_WAIT_FOREVER);
	printf("MSP: Send message: %s.\n",msg_str[msg_s.id]);

	// Send back the result	data
	unit=UNIT;
	cycle=sz_img/unit;
	residue=sz_img-unit*cycle;
	packi.unit=unit;
	packi.cycle=cycle;
	packi.residue=residue;
	nr = sendto(s, (void*)&packi, sizeof(packet_info), 0, (PSA)&sa, sz_sa);
	recvncfrom(s, (void**)&pBuf, 0, (PSA)&sa, &sz_sa, &hBuf);
	recvncfree(hBuf);

	nr_sum = 0;
	ptr = data;
	int k;
	for(k = 1; k <= cycle; ++k)
	{
		nr = sendto(s, ptr, unit, 0, (PSA)&sa, sz_sa);
		recvncfrom(s, (void**)&pBuf, 0, (PSA)&sa, &sz_sa, &hBuf);
		recvncfree(hBuf);
		ptr += unit;
		nr_sum += nr;
	}
	if(residue > 0)
	{
		nr = sendto(s, ptr, residue, 0, (PSA)&sa, sz_sa);
		recvncfrom(s, (void**)&pBuf, 0, (PSA)&sa, &sz_sa, &hBuf);
		recvncfree(hBuf);
		nr_sum += nr;
	}

	free(data);
	data=NULL;

	if(nr_sum!=sz_img)
	{
		printf("ERROR: Data transfer is incomplete (%d/%d)!\n", nr_sum, sz_img);
		return -1;
	}

	msg_s.id = MSG_SENDBACK_FINISHED;
	msg_s.img_hdr.width=0;
	msg_s.img_hdr.height=0;
	msg_s.img_hdr.data=NULL;
	Mailbox_pend(mbox_result, &msg_s, BIOS_WAIT_FOREVER);
	printf("MSP: Recv message: %s.\n",msg_str[msg_s.id]);

	return 0;
}


/* ***************************************************
* function:
*     ImageProc
*
*  description:
*
* ***************************************************
*/
void ImageProc(byte* data,int width,int height)
{
	printf("Image processing...");
	int i, size=width*height;
	byte tmp;
#pragma omp parallel for shared(data) private(i,tmp)
	for(i=0;i<size;++i)
	{
		tmp=data[i];
		data[i]=255-tmp;
	}
	printf("Done.\n");
}

//EOF
