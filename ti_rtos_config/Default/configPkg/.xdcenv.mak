#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/ccsv6/ccs_base;C:/packages;C:/products/bios_6_40_03_39/packages;C:/products/uia_2_00_01_34/packages;C:/ti/tirtos_cc32xx_2_15_00_17/packages;C:/ti/tirtos_cc32xx_2_15_00_17/products/tidrivers_cc32xx_2_15_00_26/packages;C:/ti/tirtos_cc32xx_2_15_00_17/products/bios_6_45_00_20/packages;C:/ti/tirtos_cc32xx_2_15_00_17/products/uia_2_00_02_39/packages
override XDCROOT = C:/ti/xdctools_3_31_01_33_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/ccsv6/ccs_base;C:/packages;C:/products/bios_6_40_03_39/packages;C:/products/uia_2_00_01_34/packages;C:/ti/tirtos_cc32xx_2_15_00_17/packages;C:/ti/tirtos_cc32xx_2_15_00_17/products/tidrivers_cc32xx_2_15_00_26/packages;C:/ti/tirtos_cc32xx_2_15_00_17/products/bios_6_45_00_20/packages;C:/ti/tirtos_cc32xx_2_15_00_17/products/uia_2_00_02_39/packages;C:/ti/xdctools_3_31_01_33_core/packages;..
HOSTOS = Windows
endif
