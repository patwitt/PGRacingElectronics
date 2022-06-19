################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Application/DBW/Src/DBW.c \
../Core/Src/Application/DBW/Src/PID.c \
../Core/Src/Application/DBW/Src/PIDautoTune.c 

OBJS += \
./Core/Src/Application/DBW/Src/DBW.o \
./Core/Src/Application/DBW/Src/PID.o \
./Core/Src/Application/DBW/Src/PIDautoTune.o 

C_DEPS += \
./Core/Src/Application/DBW/Src/DBW.d \
./Core/Src/Application/DBW/Src/PID.d \
./Core/Src/Application/DBW/Src/PIDautoTune.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Application/DBW/Src/%.o Core/Src/Application/DBW/Src/%.su: ../Core/Src/Application/DBW/Src/%.c Core/Src/Application/DBW/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Core/Src/Application/DBW/Inc -I../Core/Src/Application/GearControl/Inc -I../Core/Src/Middleware/Drivers/Inc -I../Core/Src/PlatformLow/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Application-2f-DBW-2f-Src

clean-Core-2f-Src-2f-Application-2f-DBW-2f-Src:
	-$(RM) ./Core/Src/Application/DBW/Src/DBW.d ./Core/Src/Application/DBW/Src/DBW.o ./Core/Src/Application/DBW/Src/DBW.su ./Core/Src/Application/DBW/Src/PID.d ./Core/Src/Application/DBW/Src/PID.o ./Core/Src/Application/DBW/Src/PID.su ./Core/Src/Application/DBW/Src/PIDautoTune.d ./Core/Src/Application/DBW/Src/PIDautoTune.o ./Core/Src/Application/DBW/Src/PIDautoTune.su

.PHONY: clean-Core-2f-Src-2f-Application-2f-DBW-2f-Src

