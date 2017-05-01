################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MCIoT_ACMP.c \
../src/MCIoT_ADC.c \
../src/MCIoT_CMU.c \
../src/MCIoT_DMA.c \
../src/MCIoT_GPIO.c \
../src/MCIoT_I2C.c \
../src/MCIoT_LESENSE_LETouch.c \
../src/MCIoT_LESENSE_Main.c \
../src/MCIoT_LETimer.c \
../src/MCIoT_LEUART.c \
../src/MCIoT_Sleep.c \
../src/MCIoT_Timer.c \
../src/MCIoT_main.c \
../src/dmactrl.c 

OBJS += \
./src/MCIoT_ACMP.o \
./src/MCIoT_ADC.o \
./src/MCIoT_CMU.o \
./src/MCIoT_DMA.o \
./src/MCIoT_GPIO.o \
./src/MCIoT_I2C.o \
./src/MCIoT_LESENSE_LETouch.o \
./src/MCIoT_LESENSE_Main.o \
./src/MCIoT_LETimer.o \
./src/MCIoT_LEUART.o \
./src/MCIoT_Sleep.o \
./src/MCIoT_Timer.o \
./src/MCIoT_main.o \
./src/dmactrl.o 

C_DEPS += \
./src/MCIoT_ACMP.d \
./src/MCIoT_ADC.d \
./src/MCIoT_CMU.d \
./src/MCIoT_DMA.d \
./src/MCIoT_GPIO.d \
./src/MCIoT_I2C.d \
./src/MCIoT_LESENSE_LETouch.d \
./src/MCIoT_LESENSE_Main.d \
./src/MCIoT_LETimer.d \
./src/MCIoT_LEUART.d \
./src/MCIoT_Sleep.d \
./src/MCIoT_Timer.d \
./src/MCIoT_main.d \
./src/dmactrl.d 


# Each subdirectory must supply rules for building sources it contributes
src/MCIoT_ACMP.o: ../src/MCIoT_ACMP.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_ACMP.d" -MT"src/MCIoT_ACMP.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_ADC.o: ../src/MCIoT_ADC.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_ADC.d" -MT"src/MCIoT_ADC.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_CMU.o: ../src/MCIoT_CMU.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_CMU.d" -MT"src/MCIoT_CMU.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_DMA.o: ../src/MCIoT_DMA.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_DMA.d" -MT"src/MCIoT_DMA.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_GPIO.o: ../src/MCIoT_GPIO.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_GPIO.d" -MT"src/MCIoT_GPIO.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_I2C.o: ../src/MCIoT_I2C.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_I2C.d" -MT"src/MCIoT_I2C.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_LESENSE_LETouch.o: ../src/MCIoT_LESENSE_LETouch.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_LESENSE_LETouch.d" -MT"src/MCIoT_LESENSE_LETouch.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_LESENSE_Main.o: ../src/MCIoT_LESENSE_Main.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_LESENSE_Main.d" -MT"src/MCIoT_LESENSE_Main.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_LETimer.o: ../src/MCIoT_LETimer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_LETimer.d" -MT"src/MCIoT_LETimer.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_LEUART.o: ../src/MCIoT_LEUART.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_LEUART.d" -MT"src/MCIoT_LEUART.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_Sleep.o: ../src/MCIoT_Sleep.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_Sleep.d" -MT"src/MCIoT_Sleep.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_Timer.o: ../src/MCIoT_Timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_Timer.d" -MT"src/MCIoT_Timer.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/MCIoT_main.o: ../src/MCIoT_main.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/MCIoT_main.d" -MT"src/MCIoT_main.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/dmactrl.o: ../src/dmactrl.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32LG990F256=1' '-DDEBUG=1' -I"/Users/pavandhareshwar/SimplicityStudio/workspace_2/LeopardGecko_Slave_Code/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/CMSIS/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/bsp" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/emlib/inc" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/common/drivers" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//platform/Device/SiliconLabs/EFM32LG/Include" -I"/Applications/Simplicity Studio.app/Contents/Eclipse/developer/sdks/exx32/v5.0.0.0//hardware/kit/EFM32LG_STK3600/config" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"src/dmactrl.d" -MT"src/dmactrl.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


