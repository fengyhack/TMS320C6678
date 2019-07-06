#
_XDCBUILDCOUNT = 0
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = D:/Texas/SDK/bios_6_33_06_50/packages;D:/Texas/SDK/imglib_c66x_3_1_1_0/packages;D:/Texas/SDK/omp_1_01_03_02/packages;D:/Texas/SDK/ipc_1_24_03_32/packages;D:/Texas/SDK/pdk_C6678_1_1_2_6/packages;D:/Texas/SDK/ndk_2_21_02_43/packages;D:/Texas/SDK/mcsdk_2_01_02_06;D:/Texas/SDK/mcsdk_2_01_02_06/demos;D:/Texas/CCS/ccsv6/ccs_base;E:/CCSv6/image_processing_openmp_evmc6678l/.config
override XDCROOT = D:/Texas/SDK/xdctools_3_23_04_60
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = D:/Texas/SDK/bios_6_33_06_50/packages;D:/Texas/SDK/imglib_c66x_3_1_1_0/packages;D:/Texas/SDK/omp_1_01_03_02/packages;D:/Texas/SDK/ipc_1_24_03_32/packages;D:/Texas/SDK/pdk_C6678_1_1_2_6/packages;D:/Texas/SDK/ndk_2_21_02_43/packages;D:/Texas/SDK/mcsdk_2_01_02_06;D:/Texas/SDK/mcsdk_2_01_02_06/demos;D:/Texas/CCS/ccsv6/ccs_base;E:/CCSv6/image_processing_openmp_evmc6678l/.config;D:/Texas/SDK/xdctools_3_23_04_60/packages;..
HOSTOS = Windows
endif
