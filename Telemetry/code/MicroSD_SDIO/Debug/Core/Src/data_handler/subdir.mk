################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/data_handler/handler.c 

OBJS += \
./Core/Src/data_handler/handler.o 

C_DEPS += \
./Core/Src/data_handler/handler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/data_handler/%.o Core/Src/data_handler/%.su: ../Core/Src/data_handler/%.c Core/Src/data_handler/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-data_handler

clean-Core-2f-Src-2f-data_handler:
	-$(RM) ./Core/Src/data_handler/handler.d ./Core/Src/data_handler/handler.o ./Core/Src/data_handler/handler.su

.PHONY: clean-Core-2f-Src-2f-data_handler

