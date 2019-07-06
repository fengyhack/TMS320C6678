## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd \
  package/cfg/image_processing_openmp_evmc6678l_pe66.oe66 \

linker.cmd: package/cfg/image_processing_openmp_evmc6678l_pe66.xdl
	$(SED) 's"^\"\(package/cfg/image_processing_openmp_evmc6678l_pe66cfg.cmd\)\"$""\"E:/CCSv6/image_processing_openmp_evmc6678l/.config/xconfig_image_processing_openmp_evmc6678l/\1\""' package/cfg/image_processing_openmp_evmc6678l_pe66.xdl > $@
