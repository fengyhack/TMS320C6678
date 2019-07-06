/****************************************************************************
 * Copyright (c) 2011 Texas Instruments Incorporated - http://www.ti.com
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *****************************************************************************/

/*
 *   @file  resourcemgr.c
 *
 *   @brief
 *      This file implements the common configuration of the resource manager for
 *      the application to use.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "ti/platform/resource_mgr.h"

/* PASS RL file */
#include <ti/csl/cslr_pa_ss.h>

/* Firmware images */
#include <ti/drv/qmss/qmss_firmware.h>
#include <ti/drv/pa/fw/classify1_bin.c>
#include <ti/drv/pa/fw/classify2_bin.c>
#include <ti/drv/pa/fw/pam_bin.c>

/* PA definitions */
#define     MAX_NUM_L2_HANDLES          10
#define     MAX_NUM_L3_HANDLES          20
#define     MAX_NUM_L4_HANDLES          40

#define     BUFSIZE_PA_INST             256
#define     BUFSIZE_L2_TABLE            1000
#define     BUFSIZE_L3_TABLE            4000

/**********************************************************************
 ************************** Global Variables **************************
 **********************************************************************/
/* Host Descriptor Region - [Size of descriptor * Number of descriptors]
 *
 * MUST be cache line byte aligned.
 */
#pragma DATA_SECTION(gHostDesc, ".resmgr_memregion");
#pragma DATA_ALIGN (gHostDesc, 128)
uint8_t         gHostDesc[MAX_DESC_SIZE * MAX_NUM_DESC];

/* This is the global free queue for QMSS. It holds all free descriptors which can be used */
#pragma DATA_SECTION(gGlobalFreeQHnd, ".resmgr_handles");
Qmss_QueueHnd   gGlobalFreeQHnd;

/* Channels handles for CPPI DMA support of the Packet Accelerator (PA) */
#pragma DATA_SECTION(gPassCpdmaTxChanHnd, ".resmgr_handles");
#pragma DATA_SECTION(gPassCpdmaRxChanHnd, ".resmgr_handles");
#pragma DATA_SECTION(gPassCpdmaHnd, 	  ".resmgr_handles");
Cppi_ChHnd      gPassCpdmaTxChanHnd [NUM_PA_TX_QUEUES];
Cppi_ChHnd      gPassCpdmaRxChanHnd [NUM_PA_RX_CHANNELS];
Cppi_Handle     gPassCpdmaHnd;

/* PA Driver Handle */
#pragma DATA_SECTION(gPAInstHnd, ".resmgr_handles");
Pa_Handle       gPAInstHnd;

/* PA instance */
#pragma DATA_SECTION(gPAInst, ".resmgr_pa");
#pragma DATA_ALIGN(gPAInst, 8)
uint8_t         gPAInst[BUFSIZE_PA_INST];

/* Memory used for PA handles */
#pragma DATA_SECTION(gMemL2Ram, ".resmgr_pa");
#pragma DATA_ALIGN(gMemL2Ram, 8)
uint8_t     gMemL2Ram[BUFSIZE_L2_TABLE];

#pragma DATA_SECTION(gMemL3Ram, ".resmgr_pa");
#pragma DATA_ALIGN(gMemL3Ram, 8)
uint8_t     gMemL3Ram[BUFSIZE_L3_TABLE];

/* Global debug counters */
#pragma DATA_SECTION(gQPopErrorCounter, ".resmgr_handles");
uint32_t gQPopErrorCounter;
#pragma DATA_SECTION(gQPushErrorCounter, ".resmgr_handles")
uint32_t gQPushErrorCounter;

/* Configuration Information */
CPPI_CFG_T 		gCppiCfg[MAX_CPPI_CFG];



/*
 * The device specific configuration and initialization routines
 * for QMSS Low Level Driver. If the user wants to change the
 * default configurations, the below file needs to be modified.
 */
#include <ti/drv/qmss/device/qmss_device.c>

/*
 * The device specific configuration and initialization routines
 * for CPPI Low Level Driver. If the user wants to change the
 * default configurations, the below file needs to be modified.
 */
#include <ti/drv/cppi/device/cppi_device.c>


/**********************************************************************
 ************ Resource Manager QMSS configuration Functions ************
 **********************************************************************/

/** ============================================================================
 *   @n@b res_mgr_qmss_freeq
 *
 *   @b Description
 *   @n This API returns the handle to the global free queue for QMSS. The global free
 *   queue maintains all usable descriptors for anyone using QMSS.
 *
 *   @param[in]  None
 *
 *   @return
 * =============================================================================
 */
Qmss_QueueHnd
res_mgr_qmss_get_freeq (void)
{
	return gGlobalFreeQHnd;
}

 /** ============================================================================
 *   @n@b QMSS_QPOP
 *
 *   @b Description
 *   @n This API implements a the queue pop logic with the Cache Management
 *
 *   @param[in]  handler
 *        Queue handler
 *   @param[in]  type
 *        Queue handler type
 *   @param[in]  pHostDescriptor
 *       pointer to  descriptor 
 * 
 *   @return    
 *   @n None
 * =============================================================================
 */
int32_t 
QMSS_QPOP
(
    Qmss_QueueHnd           handler,  
    QHANDLER_TYPE           type,
    Cppi_HostDesc**         pHostDescriptor
) 
{

    Cppi_HostDesc*      pHostDesc = *pHostDescriptor;

    pHostDesc = Qmss_queuePop (handler);

    /* Get a Tx free descriptor to send a command to the PA PDSP */
    if (pHostDesc == NULL)
    {
        gQPopErrorCounter++;
        return -1;
    }
    /* The descriptor address returned from the hardware has the 
     * descriptor size appended to the address in the last 4 bits.
     *
     * To get the true descriptor size, always mask off the last 
     * 4 bits of the address.
     */
    pHostDesc = (Cppi_HostDesc *) ((uint32_t) pHostDesc & 0xFFFFFFF0);
    *pHostDescriptor = pHostDesc;

    /* Inv cache based on the qhandler type*/
    CACHE_invL1d((void *)pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
    CACHE_invL2((void *) pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);

    if (type != QHANDLER_QPOP_FDQ_NO_ATTACHEDBUF) {
        /* Not needed for FDQ with no attached buffers pop */
        CACHE_invL1d((void *)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
        CACHE_invL2((void *) pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    }


    return 0;

}

/** ============================================================================
 *   @n@b QMSS_QPUSH
 *
 *   @b Description
 *   @n This API implements a the queue push logic with the Cache Management
 *
 *   @param[in]  handler
 *        Queue handler
 *   @param[in]  descAddr
 *        Descriptor address
 *   @param[in]  packetSize
 *        packet Size
 *   @param[in]  descSize
 *        descriptor Size
 *   @param[in]  location
 *        Qmss_Location location
 *   @return    
 *   @n None
 * =============================================================================
 */
void 
QMSS_QPUSH 
(
    Qmss_QueueHnd          handler,
    void                   *descAddr, 
    uint32_t               packetSize, 
    uint32_t               descSize, 
    Qmss_Location          location 
)
{

    Cppi_HostDesc*         pHostDesc = (Cppi_HostDesc *) descAddr;
    
    if ( descAddr == NULL ) {
        gQPushErrorCounter ++;
        return;
    }    
    /* Wb data cache */
    CACHE_wbL1d((void *)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    CACHE_wbL2((void *) pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    CACHE_wbL1d((void *)pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
    CACHE_wbL2((void *) pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);

    Qmss_queuePushDescSize (handler, 
                    pHostDesc, 
                    descSize 
                   );
}

/** ============================================================================
 *   @n@b QMSS_QPUSHDESCSIZE
 *
 *   @b Description
 *   @n This API implements a the queue push logic with the Cache Management
 *
 *   @param[in]  handler
 *        Queue handler
 *   @param[in]  descAddr
 *        Descriptor address
 *   @param[in]  descSize
 *        descriptor Size
 *   @return    
 *   @n None
 * =============================================================================
 */
void 
QMSS_QPUSHDESCSIZE 
(
    Qmss_QueueHnd          handler,
    void                   *descAddr, 
    uint32_t               descSize
)
{

    Cppi_HostDesc*         pHostDesc = (Cppi_HostDesc *) descAddr;

    if ( descAddr == NULL ) {
        gQPushErrorCounter++;
        return;
    }
    /* Wb data cache */
    CACHE_wbL1d((void *)pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    CACHE_wbL2((void *) pHostDesc->buffPtr, pHostDesc->buffLen, CACHE_WAIT);
    CACHE_wbL1d((void *)pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);
    CACHE_wbL2((void *) pHostDesc, sizeof(Cppi_HostDesc), CACHE_WAIT);

    Qmss_queuePushDescSize (handler, 
                    pHostDesc, 
                    descSize 
                   );
}

int32_t 
res_mgr_init_qmss_global(
    uint32_t        max_num_desc
)
{
    int32_t                     result;
    Qmss_InitCfg                qmssInitConfig;
    
    /* Initialize QMSS */
    memset (&qmssInitConfig, 0, sizeof (Qmss_InitCfg));
    
    /* Set up QMSS configuration */
    
    /* Use internal linking RAM */
    qmssInitConfig.linkingRAM0Base  =   0;
    qmssInitConfig.linkingRAM0Size  =   0;
    qmssInitConfig.linkingRAM1Base  =   0x0;
    qmssInitConfig.maxDescNum       =   max_num_desc;
    
    qmssInitConfig.pdspFirmware[0].pdspId = Qmss_PdspId_PDSP1;
    #if 1 /* LE Mode */
    qmssInitConfig.pdspFirmware[0].firmware = (void *) &acc48_le;
    qmssInitConfig.pdspFirmware[0].size = sizeof (acc48_le);
    #else
    qmssInitConfig.pdspFirmware[0].firmware = (void *) &acc48_be;
    qmssInitConfig.pdspFirmware[0].size = sizeof (acc48_be);
    #endif
    
    /* Initialize the Queue Manager */
    result = Qmss_init (&qmssInitConfig, &qmssGblCfgParams[0]);
    if (result != QMSS_SOK)
    {
        /* Error initializing Queue Manager SubSystem */
        return -1;
    }

    /* Start Queue manager on this core */
    Qmss_start ();

    return 0;

}

/** ============================================================================
 *   @n@b res_mgr_init_qmss
 *
 *   @b Description
 *   @n This API initializes the QMSS LLD.
 *
 *   @param[in]  p_qmss_cfg
 *        Pointer to QMSS_CFG_T
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t 
res_mgr_init_qmss 
(
    QMSS_CFG_T      *p_qmss_cfg
)
{
    int32_t                     result;
    Qmss_MemRegInfo             memCfg;
    Cppi_DescCfg                cppiDescCfg;
    uint32_t                    numAllocated;

    /* Setup the descriptor memory regions.
     *
     * The Descriptor base addresses MUST be global addresses and
     * all memory regions MUST be setup in ascending order of the
     * descriptor base addresses.
     */

    /* Initialize and setup CPSW Host Descriptors required for example */
    memset (gHostDesc, 0, p_qmss_cfg->desc_size * p_qmss_cfg->max_num_desc);
    memCfg.descBase             =   (uint32_t *) Convert_CoreLocal2GlobalAddr ((uint32_t) gHostDesc);
    memCfg.descSize             =   p_qmss_cfg->desc_size;
    memCfg.descNum              =   p_qmss_cfg->max_num_desc;
    memCfg.manageDescFlag       =   Qmss_ManageDesc_MANAGE_DESCRIPTOR;
    memCfg.memRegion            =   p_qmss_cfg->mem_region;
    memCfg.startIndex           =   0;

    /* Insert Host Descriptor memory region */
    result = Qmss_insertMemoryRegion(&memCfg);
    if (result == QMSS_MEMREGION_ALREADY_INITIALIZED)
    {
    	/* Memory Region already Initialized */
    }
    else if (result < QMSS_SOK)
    {
    	/* Error: Inserting memory region */
        return -1;
    } else if (memCfg.memRegion == Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED)
    {
        memCfg.memRegion = (Qmss_MemRegion) result;
    }
    
    /* Initialize all the descriptors we just allocated on the
     * memory region above. Setup the descriptors with some well
     * known values before we use them for data transfers.
     */
    memset (&cppiDescCfg, 0, sizeof (cppiDescCfg));
    cppiDescCfg.memRegion       =   memCfg.memRegion;
    cppiDescCfg.descNum         =   p_qmss_cfg->max_num_desc;
    cppiDescCfg.destQueueNum    =   QMSS_PARAM_NOT_SPECIFIED;
    cppiDescCfg.queueType       =   Qmss_QueueType_GENERAL_PURPOSE_QUEUE;
    cppiDescCfg.initDesc        =   Cppi_InitDesc_INIT_DESCRIPTOR;
    cppiDescCfg.descType        =   Cppi_DescType_HOST;

    /* By default:
     *      (1) Return descriptors to tail of queue
     *      (2) Always return entire packet to this free queue
     *      (3) Set that PS Data is always present in start of SOP buffer
     *      (4) Configure free q num < 4K, hence qMgr = 0
     *      (5) Recycle back to the same Free queue by default.
     */
    cppiDescCfg.returnPushPolicy            =   Qmss_Location_TAIL;
    cppiDescCfg.cfg.host.returnPolicy       =   Cppi_ReturnPolicy_RETURN_ENTIRE_PACKET;
    cppiDescCfg.cfg.host.psLocation         =   Cppi_PSLoc_PS_IN_DESC;
    cppiDescCfg.returnQueue.qMgr            =   0;
    cppiDescCfg.returnQueue.qNum            =   QMSS_PARAM_NOT_SPECIFIED;
    cppiDescCfg.epibPresent                 =   Cppi_EPIB_EPIB_PRESENT;

    /* Initialize the descriptors, create a free queue and push descriptors to a global free queue */
    if ((gGlobalFreeQHnd = Cppi_initDescriptor (&cppiDescCfg, &numAllocated)) <= 0)
    {
        /* Error Initializing Free Descriptors, Error: %d \n", gGlobalFreeQHnd */
        return -1;
    }

    if (numAllocated != cppiDescCfg.descNum)  {
      return (-1);
    }

    /* Queue Manager Initialization Done */
    return 0;
}

/** ============================================================================
 *   @n@b res_mgr_stop_qmss
 *
 *   @b Description
 *   @n This API de-initializes the QMSS LLD.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t 
res_mgr_stop_qmss 
(
    void
)
{
    Qmss_queueClose (gGlobalFreeQHnd);

    return 0;
}

/**********************************************************************
 ************ Resoure Manager CPPI configuration Functions ************
 **********************************************************************/

/** ============================================================================
 *   @n@b res_mgr_cppi_get_passhandle
 *
 *   @b Description
 *   @n This API returns the handle to the the CPPI DMA for the Packet Accelerator (PA).
 *
 *   @param[in]  None
 *
 *   @return  Cppi_Handle
 * =============================================================================
 */
Cppi_Handle
res_mgr_cppi_get_passhandle (void){
	return gPassCpdmaHnd;
}


/** ============================================================================
 *   @n@b res_mgr_init_cppi
 *
 *   @b Description
 *   @n This API initializes the CPPI LLD, opens the PDMA and opens up
 *      the Tx, Rx channels required for data transfers.
 *
 *   @param[in]  p_qmss_cfg
 *        Pointer to CPPI_CFG_T
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t 
res_mgr_init_cppi 
(
    CPPI_CFG_T      *p_cppi_cfg
)
{
    int32_t                     result, i;
    Cppi_CpDmaInitCfg           cpdmaCfg;
    uint8_t                     isAllocated;
    Cppi_TxChInitCfg            txChCfg;
    Cppi_RxChInitCfg            rxChInitCfg;
    Cppi_Handle                 *p_handle = NULL;
    Cppi_ChHnd                  *p_ch_handle = NULL;

    if (p_cppi_cfg->master_core)
    {
        /* Initialize CPPI LLD */
        result = Cppi_init (&cppiGblCfgParams[0]);
        if (result != CPPI_SOK)
        {
            platform_write ("Error initializing CPPI LLD, Error code : %d\n", result);
            return -1;
        }
    }

    /* Initialize CPDMA */
    memset (&cpdmaCfg, 0, sizeof (Cppi_CpDmaInitCfg));
    cpdmaCfg.dmaNum     = p_cppi_cfg->dma_num;
    if (cpdmaCfg.dmaNum == Cppi_CpDma_PASS_CPDMA)
    {
        p_handle = &gPassCpdmaHnd;
        memcpy(&gCppiCfg[CPPI_CFG_PASS], p_cppi_cfg, sizeof(CPPI_CFG_T));
    }

    if (p_handle)
    {
        if ((*p_handle = Cppi_open (&cpdmaCfg)) == NULL)
        {
            platform_write ("Error initializing CPPI for CPDMA %d \n", cpdmaCfg.dmaNum);
            return -1;
        }
    }
    else
    {
        return -1;
    }

    /* Open all CPPI Tx Channels. These will be used to send data to PASS/CPSW */
    for (i = 0; i < p_cppi_cfg->num_tx_queues; i ++)
    {
        txChCfg.channelNum      =   i;       /* CPPI channels are mapped one-one to the PA Tx queues */
        txChCfg.txEnable        =   Cppi_ChState_CHANNEL_DISABLE;  /* Disable the channel for now. */
        txChCfg.filterEPIB      =   0;
        txChCfg.filterPS        =   0;
        txChCfg.aifMonoMode     =   0;
        txChCfg.priority        =   2;

        if (cpdmaCfg.dmaNum == Cppi_CpDma_PASS_CPDMA)
        {
            p_ch_handle = &gPassCpdmaTxChanHnd[i];
        }

        if (p_ch_handle == NULL)
        {
            return -1;
        }

        if ((*p_ch_handle = Cppi_txChannelOpen (*p_handle, &txChCfg, &isAllocated)) == NULL)
        {
            platform_write ("Error opening Tx channel %d\n", txChCfg.channelNum);
            return -1;
        }

        Cppi_channelEnable (*p_ch_handle);
    }

    /* Open all CPPI Rx channels. These will be used by PA to stream data out. */
    for (i = 0; i < p_cppi_cfg->num_rx_channels; i++)
    {
        /* Open a CPPI Rx channel that will be used by PA to stream data out. */
        rxChInitCfg.channelNum  =   i;
        rxChInitCfg.rxEnable    =   Cppi_ChState_CHANNEL_DISABLE;
        if ((gPassCpdmaRxChanHnd[i] = Cppi_rxChannelOpen (*p_handle, &rxChInitCfg, &isAllocated)) == NULL)
        {
        	platform_write("Error opening Rx channel: %d \n", rxChInitCfg.channelNum);
            return -1;
        }

        /* Also enable Rx Channel */
        Cppi_channelEnable (gPassCpdmaRxChanHnd[i]);
    }

    /* Clear CPPI Loobpack bit in PASS CDMA Global Emulation Control Register */
    Cppi_setCpdmaLoopback(*p_handle, 0);

    /* CPPI Init Done. Return success */
    return 0;
}

/** ============================================================================
 *   @n@b res_mgr_stop_cppi
 *
 *   @b Description
 *   @n This API de-initializes the CPPI LLD.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
int32_t 
res_mgr_stop_cppi 
(
    CPPI_CFG_TYPE       cfg_type
)
{
    uint32_t i;

    if (cfg_type == CPPI_CFG_PASS)
    {
        for (i = 0; i < gCppiCfg[cfg_type].num_rx_channels; i++)
            Cppi_channelClose (gPassCpdmaRxChanHnd[i]);
        
        for (i = 0; i < gCppiCfg[cfg_type].num_tx_queues; i++)
            Cppi_channelClose (gPassCpdmaTxChanHnd[i]);
    }

    return 0;
}


/**********************************************************************
 ************ Resource Manager PA configuration Functions ************
 **********************************************************************/
/** ============================================================================
 *   @n@b Download_PAFirmware
 *
 *   @b Description
 *   @n This API downloads the PA firmware required for PDSP operation.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */
static int32_t Download_PAFirmware (Void)
{
    int32_t                   i;

    /* Hold the PA in reset state during download */
    Pa_resetControl (gPAInstHnd, pa_STATE_RESET);

    /* PDPSs 0-2 use image c1 */
    for (i = 0; i < 3; i++)
    {
        Pa_downloadImage (gPAInstHnd, i, (Ptr)c1, sizeof(c1));
    }

    /* PDSP 3 uses image c2 */
    Pa_downloadImage (gPAInstHnd, 3, (Ptr)c2, sizeof(c2));

    /* PDSPs 4-5 use image m */
    for (i = 4; i < 6; i++)
    {
        Pa_downloadImage (gPAInstHnd, i, (Ptr)m, sizeof(m));
    }

    /* Enable the PA back */
    Pa_resetControl (gPAInstHnd, pa_STATE_ENABLE);

    return 0;
}

/** ============================================================================
 *   @n@b res_mgr_get_painstance
 *
 *   @b Description
 *   @n This API returns the handle to the PA.
 *
 *   @param[in]  None
 *
 *   @return  Pa_Handle
 * =============================================================================
 */
Pa_Handle
res_mgr_get_painstance (void) {
	return gPAInstHnd;
}

/* ============================================================================
 *   @n@b res_mgr_init_pass
 *
 *   @b Description
 *   @n This API initializes the PASS/PDSP.
 *
 *   @param[in]
 *   @n None
 *
 *   @return    int32_t
 *              -1      -   Error
 *              0       -   Success
 * =============================================================================
 */

int32_t 
res_mgr_init_pass 
(
    void
)
{
    paSizeInfo_t                paSize = {0};
    paConfig_t                  paCfg = {0};
    int32_t                     retVal;
    int32_t                     sizes[pa_N_BUFS];
    int32_t                     aligns[pa_N_BUFS];
    void*                       bases[pa_N_BUFS];
    
    /* Allocate space for the PA LLD buffers. The buffers we need to
    * allocate space are:
    *      (1) PA LLD Instance Info Handle
    *      (2) PA LLD L2 Handle database
    *      (3) PA LLD L3 Handle database
    */
    paSize.nMaxL2 = MAX_NUM_L2_HANDLES;
    paSize.nMaxL3 = MAX_NUM_L3_HANDLES;
    if ((retVal = Pa_getBufferReq(&paSize, sizes, aligns)) != pa_OK)
    {
        platform_write ("Pa_getBufferReq returned error %d\n", retVal);
        return -1;
    }
    
    /* Validate the buffer allocations */
    /* The first buffer is always the instance buffer */
    if ((uint32_t)gPAInst & (aligns[0] - 1))
    {
        platform_write("Pa_getBufferReq requires %d alignment for instance buffer, but address is 0x%08x\n", aligns[0], (uint32_t)gPAInst);
        return -1;
    }
    
    if (sizeof(gPAInst) < sizes[0])
    {
        platform_write ("Pa_getBufferReq requires %d bytes for instance buffer, have only %d\n", sizes[0], sizeof(gPAInst));
        return -1;
    }
    
    bases[0]    =   (Void *)gPAInst;
    
    /* The second buffer is the L2 table */
    if ((uint32_t)gMemL2Ram & (aligns[1] - 1))
    {
        platform_write ("Pa_getBufferReq requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[1], (uint32_t)gMemL2Ram);
        return (-1);
    }
    
    if (sizeof(gMemL2Ram) < sizes[1])
    {
        platform_write ("Pa_getBufferReq requires %d bytes for buffer 1, have only %d\n", sizes[1], sizeof(gMemL2Ram));
        return -1;
    }
    
    bases[1]    =   (Void *)gMemL2Ram;
    
    /* The third buffer is the L3 table */
    if ((uint32_t)gMemL3Ram & (aligns[2] - 1))
    {
        platform_write ("Pa_alloc requires %d alignment for buffer 1, but address is 0x%08x\n", aligns[2], (uint32_t)gMemL3Ram);
        return (-1);
    }
    
    if (sizeof(gMemL3Ram) < sizes[2])
    {
        platform_write ("Pa_alloc requires %d bytes for buffer 1, have only %d\n", sizes[2], sizeof(gMemL3Ram));
        return (-1);
    }
    
    bases[2]    =   (Void *)gMemL3Ram;
    
    /* Finally initialize the PA LLD */
    paCfg.initTable =   TRUE;
    paCfg.initDefaultRoute = TRUE;
    paCfg.baseAddr = CSL_PA_SS_CFG_REGS;
    paCfg.sizeCfg   =   &paSize;
    if ((retVal = Pa_create (&paCfg, bases, &gPAInstHnd)) != pa_OK)
    {
        platform_write ("Pa_create returned with error code %d\n", retVal);
        return -1;
    }
    
    /* Download the PASS PDSP firmware */
    if (Download_PAFirmware ())
    {
        return -1;
    }
    
    
    /* PA Init Done. Return success */
    return 0;
}

/**********************************************************************
 ************ Resource Manager NIMU configuration Functions ************
 **********************************************************************/

/** ============================================================================
 *   @n@b NIMU_GetConfiguration
 *
 *   @b Description
 *   @n This routine implements the NIMU_GetConfiguration API. It sets up the
 *   Ethernet properties we want and other NIMU configurable.
 *
 *   @param[in]  NIMU_CFG_T *p_cfg - Pointer to configuration structure to fill
 *
 *   @return  None
 * =============================================================================
 */
#if 0
void
NIMU_GetConfiguration (NIMU_CFG_T *p_cfg) {

    if (platform_get_macaddr(PLATFORM_MAC_TYPE_EFUSE, p_cfg->bMacAddr) != 0)
    {
        /* Can not get MAC addr from efuse, set default MAC Address */
        p_cfg->bMacAddr[0] = 0x10;
        p_cfg->bMacAddr[1] = 0x11;
        p_cfg->bMacAddr[2] = 0x12;
        p_cfg->bMacAddr[3] = 0x13;
        p_cfg->bMacAddr[4] = 0x14;
        p_cfg->bMacAddr[5] = 0x15;
    }

	return;
}
#endif
