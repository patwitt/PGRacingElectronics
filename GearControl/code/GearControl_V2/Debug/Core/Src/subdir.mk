################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Adc.c \
../Core/Src/BTN7970B.c \
../Core/Src/CANManager.c \
../Core/Src/DBW.c \
../Core/Src/Encoder.c \
../Core/Src/GearControl.c \
../Core/Src/GearSensor.c \
../Core/Src/MicroSwitch.c \
../Core/Src/PID.c \
../Core/Src/Scheduler.c \
../Core/Src/StopWatch.c \
../Core/Src/Watchdog.c \
../Core/Src/main.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/Adc.o \
./Core/Src/BTN7970B.o \
./Core/Src/CANManager.o \
./Core/Src/DBW.o \
./Core/Src/Encoder.o \
./Core/Src/GearControl.o \
./Core/Src/GearSensor.o \
./Core/Src/MicroSwitch.o \
./Core/Src/PID.o \
./Core/Src/Scheduler.o \
./Core/Src/StopWatch.o \
./Core/Src/Watchdog.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/Adc.d \
./Core/Src/BTN7970B.d \
./Core/Src/CANManager.d \
./Core/Src/DBW.d \
./Core/Src/Encoder.d \
./Core/Src/GearControl.d \
./Core/Src/GearSensor.d \
./Core/Src/MicroSwitch.d \
./Core/Src/PID.d \
./Core/Src/Scheduler.d \
./Core/Src/StopWatch.d \
./Core/Src/Watchdog.d \
./Core/Src/main.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../USB_DEVICE/App -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Adc.d ./Core/Src/Adc.o ./Core/Src/Adc.su ./Core/Src/BTN7970B.d ./Core/Src/BTN7970B.o ./Core/Src/BTN7970B.su ./Core/Src/CANManager.d ./Core/Src/CANManager.o ./Core/Src/CANManager.su ./Core/Src/DBW.d ./Core/Src/DBW.o ./Core/Src/DBW.su ./Core/Src/Encoder.d ./Core/Src/Encoder.o ./Core/Src/Encoder.su ./Core/Src/GearControl.d ./Core/Src/GearControl.o ./Core/Src/GearControl.su ./Core/Src/GearSensor.d ./Core/Src/GearSensor.o ./Core/Src/GearSensor.su ./Core/Src/MicroSwitch.d ./Core/Src/MicroSwitch.o ./Core/Src/MicroSwitch.su ./Core/Src/PID.d ./Core/Src/PID.o ./Core/Src/PID.su ./Core/Src/Scheduler.d ./Core/Src/Scheduler.o ./Core/Src/Scheduler.su ./Core/Src/StopWatch.d ./Core/Src/StopWatch.o ./Core/Src/StopWatch.su ./Core/Src/Watchdog.d ./Core/Src/Watchdog.o ./Core/Src/Watchdog.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

