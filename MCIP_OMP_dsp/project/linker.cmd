/*
 * Do not modify this file; it is automatically generated from the template
 * linkcmd.xdt in the ti.targets.elf package and will be overwritten.
 */

/*
 * put '"'s around paths because, without this, the linker
 * considers '-' as minus operator, not a file name character.
 */


-l"E:\CCSv6\image_processing_openmp_evmc6678l\Release\configPkg\package\cfg\image_processing_openmp_evmc6678l_pe66.oe66"
-l"D:\Texas\SDK\omp_1_01_03_02\packages\ti\omp\utils\lib\release\ti.omp.utils.ae66"
-l"D:\Texas\SDK\omp_1_01_03_02\packages\ti\omp\utils\lib\release\ti.omp.utils.C6678.ae66"
-l"D:\Texas\SDK\ipc_1_24_03_32\packages\ti\sdo\ipc\lib\instrumented_e66\ipc\ipc.lib"
-l"D:\Texas\SDK\omp_1_01_03_02\packages\ti\omp\navigator\lib\release\ti.omp.navigator.ae66"
-l"D:\Texas\SDK\omp_1_01_03_02\packages\ti\omp\navigator\lib\release\ti.omp.navigatorOsal.ae66"
-l"D:\Texas\SDK\omp_1_01_03_02\packages\ti\omp\navigator\lib\release\ti.omp.navigator.C6678.ae66"
-l"D:\Texas\SDK\omp_1_01_03_02\packages\ti\omp\lib\release\ti.omp.ae66"
-l"D:\Texas\SDK\omp_1_01_03_02\packages\ti\omp\config\sysbios\lib\release\ti.omp.config.sysbios.ae66"
-l"D:\Texas\SDK\pdk_C6678_1_1_2_6\packages\ti\transport\ndk\nimu\lib\release\ti.transport.ndk.nimu.ae66"
-l"D:\Texas\SDK\pdk_C6678_1_1_2_6\packages\ti\platform\evmc6678l\platform_lib\lib\release\ti.platform.evm6678l.ae66"
-l"D:\Texas\SDK\pdk_C6678_1_1_2_6\packages\ti\drv\cppi\lib\ti.drv.cppi.ae66"
-l"D:\Texas\SDK\pdk_C6678_1_1_2_6\packages\ti\drv\qmss\lib\ti.drv.qmss.ae66"
-l"D:\Texas\SDK\pdk_C6678_1_1_2_6\packages\ti\drv\pa\lib\ti.drv.pa.ae66"
-l"D:\Texas\SDK\pdk_C6678_1_1_2_6\packages\ti\csl\lib\ti.csl.ae66"
-l"D:\Texas\SDK\pdk_C6678_1_1_2_6\packages\ti\csl\lib\ti.csl.intc.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\miniPrintf\lib\miniPrintf.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\hal\ser_stub\lib\hal_ser_stub.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\hal\timer_bios\lib\hal_timer_bios.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\os\lib\os.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\hal\userled_stub\lib\hal_userled_stub.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\hal\eth_stub\lib\hal_eth_stub.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\tools\cgi\lib\cgi.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\tools\hdlc\lib\hdlc.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\tools\console\lib\console_min.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\netctrl\lib\netctrl.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\nettools\lib\nettool.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\tools\servers\lib\servers_min.ae66"
-l"D:\Texas\SDK\ndk_2_21_02_43\packages\ti\ndk\stack\lib\stk6_ppp_pppoe.ae66"
-l"D:\Texas\SDK\bios_6_33_06_50\packages\ti\sysbios\lib\instrumented_e66\sysbios\sysbios.lib"
-l"D:\Texas\SDK\xdctools_3_23_04_60\packages\ti\targets\rts6000\lib\ti.targets.rts6000.ae66"
-l"D:\Texas\SDK\xdctools_3_23_04_60\packages\ti\targets\rts6000\lib\boot.ae66"

--retain="*(xdc.meta)"


--args 0x0
-heap  0x0
-stack 0x1000

MEMORY
{
    L2SRAM (RWX) : org = 0x800000, len = 0x60000
    MSMCSRAM (RWX) : org = 0xc000000, len = 0x200000
    MSMCSRAM_NOCACHE (RWX) : org = 0xa0200000, len = 0x200000
    DDR3 (RWX) : org = 0x80000000, len = 0x10000000
    DDR3_NOCACHE (RWX) : org = 0x90000000, len = 0x10000000
}

/*
 * Linker command file contributions from all loaded packages:
 */

/* Content from xdc.services.global (null): */

/* Content from xdc (null): */

/* Content from xdc.corevers (null): */

/* Content from xdc.shelf (null): */

/* Content from xdc.services.spec (null): */

/* Content from xdc.services.intern.xsr (null): */

/* Content from xdc.services.intern.gen (null): */

/* Content from xdc.services.intern.cmd (null): */

/* Content from xdc.bld (null): */

/* Content from ti.targets (null): */

/* Content from ti.targets.elf (null): */

/* Content from xdc.rov (null): */

/* Content from xdc.runtime (null): */

/* Content from ti.targets.rts6000 (null): */

/* Content from ti.sysbios.interfaces (null): */

/* Content from ti.sysbios.family (null): */

/* Content from ti.sysbios (null): */

/* Content from xdc.services.getset (null): */

/* Content from ti.sysbios.hal (null): */

/* Content from ti.sysbios.knl (null): */

/* Content from ti.sysbios.family.c66 (null): */

/* Content from ti.sysbios.family.c64p (null): */

/* Content from ti.ndk.rov (null): */

/* Content from ti.ndk.config (null): */

/* Content from ti.csl (null): */

/* Content from ti.drv.pa (null): */

/* Content from ti.drv.qmss (null): */

/* Content from ti.drv.cppi (null): */

/* Content from ti.platform.evmc6678l (null): */

/* Content from ti.transport.ndk (null): */

/* Content from ti.sysbios.gates (null): */

/* Content from ti.sysbios.heaps (null): */

/* Content from xdc.runtime.knl (null): */

/* Content from ti.sdo.ipc.interfaces (null): */

/* Content from ti.sysbios.syncs (null): */

/* Content from ti.sdo.ipc.family (null): */

/* Content from ti.sdo.utils (null): */

/* Content from ti.omp.config.sysbios (null): */

/* Content from ti.omp (ti/omp/linkcmd.xdt): */


SECTIONS
{
    /* 
     *  If building on C66x devices and if the codeMemory and dataMemory aren't
     *  the same segment we need to load the .neardata and .fardata (non-const
     *  initialized near/far global variables) into codeMemory and run from
     *  dataMemory.  This is because dataMemory isn't guaranteed to be writable
     *  by the loader at boot-time: it could be a non-cached MSMC alias in
     *  case it is made available only after the alias as been created at
     *  startup-time. 
     */

    GROUP: load > MSMCSRAM, run > MSMCSRAM_NOCACHE, table(BINIT)
    {
        .neardata:
        .fardata:
    }
   
    .binit: load > MSMCSRAM

    /* 
     *  The .far & .fardata sections of the RTSC genenerated 'C' file (appname)
     *  and the RTS libraries (*rts*.lib) consist of global variables that must
     *  be local to each core.  These sections are therefore placed in the
     *  'Program.platform.stackMemory' segment which is required to be set to
     *  memory local to each core.
     */
    .localfar :
    {
        "image_processing_openmp_evmc6678l_pe66.oe66" (.far)
        "*rts*.lib" (.far)
        "image_processing_openmp_evmc6678l_pe66.oe66" (.fardata)
        "*rts*.lib" (.fardata)
    } > L2SRAM


    /*
     * Place the TLS sections to avoid warnings from the linker related to
     * defailt placement.
     */
    .TI.tls         >   MSMCSRAM_NOCACHE
    .TI.tls_init    >   MSMCSRAM_NOCACHE

}
SECTIONS
{
    gomp_data                load       >> MSMCSRAM_NOCACHE
    .Nav_init	     load 	>> L2SRAM	
    .init_array: 	     load 	>> L2SRAM
    .qmss: 		     load 	>> MSMCSRAM_NOCACHE
    .shareddata:	     load	>> MSMCSRAM_NOCACHE
    .cppi: 		     load 	>> MSMCSRAM
}

/* Content from ti.omp.navigator (null): */

/* Content from ti.sysbios.timers.timer64 (null): */

/* Content from ti.sysbios.xdcruntime (null): */

/* Content from ti.sysbios.family.c62 (null): */

/* Content from ti.sysbios.family.c64p.tci6488 (null): */

/* Content from ti.ndk.config.family (null): */

/* Content from ti.sysbios.utils (null): */

/* Content from ti.catalog.c6000 (null): */

/* Content from ti.catalog (null): */

/* Content from ti.catalog.peripherals.hdvicp2 (null): */

/* Content from xdc.platform (null): */

/* Content from xdc.cfg (null): */

/* Content from ti.platforms.generic (null): */

/* Content from demos.image_processing.openmp.evmc6678l.platform (null): */

/* Content from ti.sdo.ipc (ti/sdo/ipc/linkcmd.xdt): */

SECTIONS
{
    ti.sdo.ipc.SharedRegion_0:  { . += 0x30000;} run > 0xa0200000, type = NOLOAD
    ti.sdo.ipc.SharedRegion_2:  { . += 0x1000000;} run > 0x90000000, type = NOLOAD
}


/* Content from ti.sdo.ipc.heaps (null): */

/* Content from ti.sdo.ipc.nsremote (null): */

/* Content from ti.sdo.ipc.transports (null): */

/* Content from ti.omp.utils (null): */

/* Content from ti.sdo.ipc.gates (null): */

/* Content from ti.sdo.ipc.family.c647x (null): */

/* Content from ti.sdo.ipc.notifyDrivers (null): */

/* Content from configPkg (null): */


/*
 * symbolic aliases for static instance objects
 */
xdc_runtime_Startup__EXECFXN__C = 1;
xdc_runtime_Startup__RESETFXN__C = 1;
TSK_idle = ti_sysbios_knl_Task_Object__table__V + 72;

SECTIONS
{
    .text: load >> MSMCSRAM
    .ti.decompress: load > MSMCSRAM
    .stack: load > L2SRAM
    GROUP: load > MSMCSRAM_NOCACHE
    {
        .bss:
        .neardata:
        .rodata:
    }
    .cinit: load > MSMCSRAM
    .pinit: load >> MSMCSRAM_NOCACHE
    .init_array: load > MSMCSRAM_NOCACHE
    .const: load >> MSMCSRAM
    .data: load >> MSMCSRAM_NOCACHE
    .fardata: load >> MSMCSRAM_NOCACHE
    .switch: load >> MSMCSRAM
    .sysmem: load > L2SRAM
    .far: load >> MSMCSRAM_NOCACHE
    .args: load > L2SRAM align = 0x4, fill = 0 {_argsize = 0x0; }
    .cio: load >> L2SRAM align = 0x8
    .ti.handler_table: load > MSMCSRAM_NOCACHE
    .cppi: load >> MSMCSRAM align = 0x10
    .qmss: load >> MSMCSRAM align = 0x10
    .nimu_eth_ll2: load >> MSMCSRAM align = 0x10
    .far:NDK_PACKETMEM: load >> MSMCSRAM align = 0x80
    .far:NDK_OBJMEM: load >> L2SRAM align = 0x10
    .far:WEBDATA: load >> DDR3 align = 0x8
    .resmgr_memregion: load >> MSMCSRAM align = 0x80
    .resmgr_handles: load >> MSMCSRAM align = 0x10
    .resmgr_pa: load >> MSMCSRAM align = 0x8
    ddr_heap: load >> DDR3
    .plt: load >> MSMCSRAM
    platform_lib: load >> MSMCSRAM
    .far:NDK_MMBUFFER1: load >> MSMCSRAM
    ddr: load >> DDR3_NOCACHE
    .threadprivate: load >> L2SRAM
    .vecs: load >> MSMCSRAM
    .text:_c_int00: load >> MSMCSRAM align = 0x400
    .tls_tp: load >> L2SRAM
    xdc.meta: load >> MSMCSRAM, type = COPY

}
