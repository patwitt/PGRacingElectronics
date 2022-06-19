################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/PlatformLow/Src/Adc.c \
../Core/Src/PlatformLow/Src/LED.c \
../Core/Src/PlatformLow/Src/Scheduler.c \
../Core/Src/PlatformLow/Src/StopWatch.c \
../Core/Src/PlatformLow/Src/SwTimer.c \
../Core/Src/PlatformLow/Src/Watchdog.c 

OBJS += \
./Core/Src/PlatformLow/Src/Adc.o \
./Core/Src/PlatformLow/Src/LED.o \
./Core/Src/PlatformLow/Src/Scheduler.o \
./Core/Src/PlatformLow/Src/StopWatch.o \
./Core/Src/PlatformLow/Src/SwTimer.o \
./Core/Src/PlatformLow/Src/Watchdog.o 

C_DEPS += \
./Core/Src/PlatformLow/Src/Adc.d \
./Core/Src/PlatformLow/Src/LED.d \
./Core/Src/PlatformLow/Src/Scheduler.d \
./Core/Src/PlatformLow/Src/StopWatch.d \
./Core/Src/PlatformLow/Src/SwTimer.d \
./Core/Src/PlatformLow/Src/Watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/PlatformLow/Src/%.o Core/Src/PlatformLow/Src/%.su: ../Core/Src/PlatformLow/Src/%.c Core/Src/PlatformLow/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F405xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../Core/Src/Application/DBW/Inc -I../Core/Src/Application/GearControl/Inc -I../Core/Src/Middleware/Drivers/Inc -I../Core/Src/PlatformLow/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-PlatformLow-2f-Src

clean-Core-2f-Src-2f-PlatformLow-2f-Src:
	-$(RM) ./Core/Src/PlatformLow/Src/Adc.d ./Core/Src/PlatformLow/Src/Adc.o ./Core/Src/PlatformLow/Src/Adc.su ./Core/Src/PlatformLow/Src/LED.d ./Core/Src/PlatformLow/Src/LED.o ./Core/Src/PlatformLow/Src/LED.su ./Core/Src/PlatformLow/Src/Scheduler.d ./Core/Src/PlatformLow/Src/Scheduler.o ./Core/Src/PlatformLow/Src/Scheduler.su ./Core/Src/PlatformLow/Src/StopWatch.d ./Core/Src/PlatformLow/Src/StopWatch.o ./Core/Src/PlatformLow/Src/StopWatch.su ./Core/Src/PlatformLow/Src/SwTimer.d ./Core/Src/PlatformLow/Src/SwTimer.o ./Core/Src/PlatformLow/Src/SwTimer.su ./Core/Src/PlatformLow/Src/Watchdog.d ./Core/Src/PlatformLow/Src/Watchdog.o ./Core/Src/PlatformLow/Src/Watchdog.su

.PHONY: clean-Core-2f-Src-2f-PlatformLow-2f-Src

