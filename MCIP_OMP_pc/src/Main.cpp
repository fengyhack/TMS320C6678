/****************
*
* PC-side tool 
* http://fengyh.cn/
*
*****************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "dib.h"
#include "sockets.h"
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

const char  *TARGET = "192.168.2.100";
#define PORT  7
#define TIMEOUT 1   /* second(s) */
#define UNIT 2048

#define FD_SELECT 	{ \
	FD_ZERO(&fds); \
    FD_SET(s, &fds);  \
    select(s + 1, &fds, NULL, NULL, &timeout); }

typedef struct tag_packet_header
{	
	int widht;
	int height;
	int unit;
	int cycle;
	int residue;
}packet_header;

int main(void)
{
	system("title Ethernet0 Transfer Test");
	system("color 2e");
	printf("Remote ( %s : %d )\n", TARGET, PORT);


	char fileName[256] = { 0 };
	printf("Input filename:");
	scanf("%s", fileName);

	DIBitmap dib;
	dib.Open(fileName);
	dib.ToGrayscale(FALSE);
	BYTE* imgData = dib.GetImageData();
	int w = dib.GetWidth();
	int h = dib.GetHeight();
	printf("Image size = %d * %d\n", w, h);

	int size = w*h;
	int unit = UNIT;
	int cycle = size / unit;
	int residue = size - cycle*unit;
	char* data = (char*)imgData;

	struct in_addr dst;
	inet_pton(AF_INET, TARGET, &dst);
	unsigned short port = PORT;
	socketsStartup();
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;

	const char* reply = "HELLO";
	const int sz_reply = strlen(reply);
	const int sz_sin = sizeof(sockaddr_in);

	bind(s,(const struct sockaddr *) &sin, sz_sin);
	sin.sin_addr = dst;
	sin.sin_port = htons(port);

	struct timeval timeout;
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;

	fd_set fds;
	int i;
	char tmp[8] = { 0 };

	printf("Start sending data: %d * %d + %d = %d\n", unit, cycle, residue, size);

	time_t time_start = time(0);

	packet_header packhdr;
	packhdr.widht = w;
	packhdr.height = h;
	packhdr.unit = unit;
	packhdr.cycle = cycle;
	packhdr.residue = residue;
	sendto(s, (const char*)&packhdr, sizeof(packet_header), 0, (const struct sockaddr *) &sin, sz_sin);
	FD_SELECT
	recv(s, tmp, sz_reply, 0);

	char progress[64] = { 0 };
	memset(progress, '*', 50);
	int di = cycle / 50;
	int k = 0;

	for (i = 1; i <= cycle; ++i)
	{
		sendto(s, data, unit, 0, (const struct sockaddr *) &sin, sz_sin);
		data += unit;
		//FD_ZERO(&fds);
		//FD_SET(s, &fds);
		//select(s + 1, &fds, NULL, NULL, &timeout);
		recv(s, tmp, sz_reply, 0);
		if (i % di == 0)
		{
			if (k >= 50) k = 49;
			progress[k++] = '>';
		}
		printf("\r[PC]%s[DSP]", progress);
	}
	if (residue > 0)
	{
		sendto(s, data, residue, 0, (const struct sockaddr *) &sin, sz_sin);
		//FD_SELECT
		recv(s, tmp, sz_reply, 0);
	}
	printf("\nFinished transfer, waiting for the reply...\n");

	memset(progress, '*', 50);
	
	//FD_SELECT
	recv(s, (char *)&packhdr, sizeof(packet_header), 0);
	sendto(s, reply, sz_reply, 0, (const struct sockaddr *) &sin, sz_sin);

	unit = packhdr.unit;
	cycle = packhdr.cycle;
	residue = packhdr.residue;

	printf("Start recving data: %d * %d + %d = %d\n", unit, cycle, residue, size);

	data = (char*)imgData;
	for (i = 1; i <= cycle;++i)
	{
		//FD_SELECT
		recv(s, data, unit, 0);
		data += unit;
		sendto(s, reply, sz_reply, 0, (const struct sockaddr *) &sin, sz_sin);
		if (i % di == 0)
		{
			progress[--k] = '<';
		}
		printf("\r[PC]%s[DSP]", progress);
	}
	if (residue > 0)
	{
		//FD_SELECT
		recv(s, data, residue, 0);
		sendto(s, reply, sz_reply, 0, (const struct sockaddr *) &sin, sz_sin);
	}
	printf("\nFinished receiving the result data.\n");
	int elapsed = (int)(time(0) - time_start);
	printf("Time = %d seconds\n", elapsed);

	char sname[256] = { 0 };
	printf("Input dest file name:");
	scanf("%s", sname);

	dib.Create(imgData, w, h);
	dib.Save(sname);

	closesocket(s);
	socketsShutdown();

	system("pause");
	return 0;
}
