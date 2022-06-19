################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Middleware/Drivers/Src/CAN.c \
../Core/Src/Middleware/Drivers/Src/DCMotor.c \
../Core/Src/Middleware/Drivers/Src/MicroSwitch.c \
../Core/Src/Middleware/Drivers/Src/Servo.c 

OBJS += \
./Core/Src/Middleware/Drivers/Src/CAN.o \
./Core/Src/Middleware/Drivers/Src/DCMotor.o \
./Core/Src/Middleware/Drivers/Src/MicroSwitch.o \
./Core/Src/Middleware/Drivers/Src/Servo.o 

C_DEPS += \
./Core/Src/Middleware/Drivers/Src/CAN.d \
./Core/Src/Middleware/Drivers/Src/DCMotor.d \
./Core/Src/Middleware/Drivers/Src/MicroSwitch.d \
./Core/Src/Middleware/Drivers/Src/Servo.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Middleware/Drivers/Src/%.o Core/Src/Middleware/Drivers/Src/%.su: ../Core/Src/Middleware/Drivers/Src/%.c Core/Src/Middleware/Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Core/Src/Application/DBW/Inc -I../Core/Src/Application/GearControl/Inc -I../Core/Src/Middleware/Drivers/Inc -I../Core/Src/PlatformLow/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Middleware-2f-Drivers-2f-Src

clean-Core-2f-Src-2f-Middleware-2f-Drivers-2f-Src:
	-$(RM) ./Core/Src/Middleware/Drivers/Src/CAN.d ./Core/Src/Middleware/Drivers/Src/CAN.o ./Core/Src/Middleware/Drivers/Src/CAN.su ./Core/Src/Middleware/Drivers/Src/DCMotor.d ./Core/Src/Middleware/Drivers/Src/DCMotor.o ./Core/Src/Middleware/Drivers/Src/DCMotor.su ./Core/Src/Middleware/Drivers/Src/MicroSwitch.d ./Core/Src/Middleware/Drivers/Src/MicroSwitch.o ./Core/Src/Middleware/Drivers/Src/MicroSwitch.su ./Core/Src/Middleware/Drivers/Src/Servo.d ./Core/Src/Middleware/Drivers/Src/Servo.o ./Core/Src/Middleware/Drivers/Src/Servo.su

.PHONY: clean-Core-2f-Src-2f-Middleware-2f-Drivers-2f-Src

