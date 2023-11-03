################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/sdcard/SDCARD.c \
../Core/Src/sdcard/sdmmc.c 

OBJS += \
./Core/Src/sdcard/SDCARD.o \
./Core/Src/sdcard/sdmmc.o 

C_DEPS += \
./Core/Src/sdcard/SDCARD.d \
./Core/Src/sdcard/sdmmc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/sdcard/%.o Core/Src/sdcard/%.su Core/Src/sdcard/%.cyclo: ../Core/Src/sdcard/%.c Core/Src/sdcard/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-sdcard

clean-Core-2f-Src-2f-sdcard:
	-$(RM) ./Core/Src/sdcard/SDCARD.cyclo ./Core/Src/sdcard/SDCARD.d ./Core/Src/sdcard/SDCARD.o ./Core/Src/sdcard/SDCARD.su ./Core/Src/sdcard/sdmmc.cyclo ./Core/Src/sdcard/sdmmc.d ./Core/Src/sdcard/sdmmc.o ./Core/Src/sdcard/sdmmc.su

.PHONY: clean-Core-2f-Src-2f-sdcard

