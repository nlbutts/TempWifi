################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
configPkg/: C:/ti/CC3200SDK_1.1.0/cc3200-sdk/ti_rtos/ti_rtos_config/app.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti/xdctools_3_31_01_33_core/xs" --xdcpath="C:/ti/ccsv6/ccs_base;/packages;/products/bios_6_40_03_39/packages;/products/uia_2_00_01_34/packages;C:/ti/tirtos_cc32xx_2_15_00_17/packages;C:/ti/tirtos_cc32xx_2_15_00_17/products/tidrivers_cc32xx_2_15_00_26/packages;C:/ti/tirtos_cc32xx_2_15_00_17/products/bios_6_45_00_20/packages;C:/ti/tirtos_cc32xx_2_15_00_17/products/uia_2_00_02_39/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4 -p ti.platforms.simplelink:CC3200 -r release -c "C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7" "$<"
	@echo 'Finished building: $<'
	@echo ' '


