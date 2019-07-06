#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = D:/Texas/SDK/ipc_1_24_03_32/packages;D:/Texas/SDK/bios_6_33_06_50/packages;D:/Texas/SDK/imglib_c66x_3_1_1_0/packages;D:/Texas/SDK/mcsdk_2_01_02_06;D:/Texas/SDK/uia_1_00_03_25/packages;D:/Texas/SDK/mcsdk_2_01_02_06/demos;D:/Texas/SDK/pdk_C6678_1_1_2_6/packages;D:/Texas/CCS/ccsv6/ccs_base;E:/CCSv6/mcip_slaveA/.config
override XDCROOT = D:/Texas/SDK/xdctools_3_23_04_60
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = D:/Texas/SDK/ipc_1_24_03_32/packages;D:/Texas/SDK/bios_6_33_06_50/packages;D:/Texas/SDK/imglib_c66x_3_1_1_0/packages;D:/Texas/SDK/mcsdk_2_01_02_06;D:/Texas/SDK/uia_1_00_03_25/packages;D:/Texas/SDK/mcsdk_2_01_02_06/demos;D:/Texas/SDK/pdk_C6678_1_1_2_6/packages;D:/Texas/CCS/ccsv6/ccs_base;E:/CCSv6/mcip_slaveA/.config;D:/Texas/SDK/xdctools_3_23_04_60/packages;..
HOSTOS = Windows
endif
