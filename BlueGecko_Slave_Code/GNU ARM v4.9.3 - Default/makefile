################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include kit_lcd/BRD4100A/subdir.mk
-include kit_flashpwr/BRD4100A/subdir.mk
-include kit/BRD4100A/subdir.mk
-include glib/BRD4100A/subdir.mk
-include emlib/subdir.mk
-include device/EFR32_B_1_1_P/gcc/subdir.mk
-include bgapi/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: BlueGecko_Slave_Code.axf

# Tool invocations
BlueGecko_Slave_Code.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "/Users/pavandhareshwar/SimplicityStudio/workspace_2/BlueGecko_Slave_Code/linker/GCC/efr32bg1p232f256gm48.ld" -Xlinker -no-enum-size-warning -Xlinker -no-wchar-size-warning -Xlinker --gc-sections -Xlinker -Map="BlueGecko_Slave_Code.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -o BlueGecko_Slave_Code.axf "./advertisement.o" "./app.o" "./app_hw.o" "./app_ui.o" "./application_properties.o" "./beacon.o" "./ble-callback-stubs.o" "./ble-callbacks.o" "./gatt_db.o" "./graphics.o" "./htm.o" "./ia.o" "./main.o" "./bgapi/dmadrv.o" "./bgapi/em_adc.o" "./bgapi/em_ldma.o" "./bgapi/em_leuart.o" "./device/EFR32_B_1_1_P/gcc/startup_efr32bg1p.o" "./emlib/em_cmu.o" "./emlib/em_cryotimer.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_msc.o" "./emlib/em_rtcc.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./glib/BRD4100A/bmp.o" "./glib/BRD4100A/dmd_display.o" "./glib/BRD4100A/glib.o" "./glib/BRD4100A/glib_bitmap.o" "./glib/BRD4100A/glib_circle.o" "./glib/BRD4100A/glib_font_narrow_6x8.o" "./glib/BRD4100A/glib_font_normal_8x8.o" "./glib/BRD4100A/glib_font_number_16x20.o" "./glib/BRD4100A/glib_line.o" "./glib/BRD4100A/glib_polygon.o" "./glib/BRD4100A/glib_rectangle.o" "./glib/BRD4100A/glib_string.o" "./kit/BRD4100A/bsp_bcc.o" "./kit/BRD4100A/bsp_stk.o" "./kit/BRD4100A/bsp_stk_leds.o" "./kit/BRD4100A/i2cspm.o" "./kit/BRD4100A/si7013.o" "./kit/BRD4100A/tempsens.o" "./kit/BRD4100A/udelay.o" "./kit_flashpwr/BRD4100A/mx25flash_spi.o" "./kit_lcd/BRD4100A/display.o" "./kit_lcd/BRD4100A/displayls013b7dh03.o" "./kit_lcd/BRD4100A/displaypalemlib.o" "./src/InitDevice.o" "/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1P/GCC/bgapi.a" "/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1P/EABI/stack.a" "/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1P/EABI/binstack.o" "/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/gecko_sdk_suite/v1.0//protocol/bluetooth_2.3/lib/EFR32BG1P/EABI/binbootloader.o" -lm -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: BlueGecko_Slave_Code.hex'
	arm-none-eabi-objcopy -O ihex "BlueGecko_Slave_Code.axf" "BlueGecko_Slave_Code.hex"
	@echo ' '

	@echo 'Building bin file: BlueGecko_Slave_Code.bin'
	arm-none-eabi-objcopy -O binary "BlueGecko_Slave_Code.axf" "BlueGecko_Slave_Code.bin"
	@echo ' '

	@echo 'Building s37 file: BlueGecko_Slave_Code.s37'
	arm-none-eabi-objcopy -O srec "BlueGecko_Slave_Code.axf" "BlueGecko_Slave_Code.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "BlueGecko_Slave_Code.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) BlueGecko_Slave_Code.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
