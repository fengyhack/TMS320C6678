## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e66 linker.cmd \
  package/cfg/mcip_master_pe66.oe66 \

linker.cmd: package/cfg/mcip_master_pe66.xdl
	$(SED) 's"^\"\(package/cfg/mcip_master_pe66cfg.cmd\)\"$""\"E:/Studio/TICCS/mcip_master/.config/xconfig_mcip_master/\1\""' package/cfg/mcip_master_pe66.xdl > $@
