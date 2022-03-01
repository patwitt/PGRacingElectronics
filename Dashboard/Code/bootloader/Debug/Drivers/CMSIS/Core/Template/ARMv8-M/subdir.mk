################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Core/Template/ARMv8-M/main_s.c \
../Drivers/CMSIS/Core/Template/ARMv8-M/tz_context.c 

OBJS += \
./Drivers/CMSIS/Core/Template/ARMv8-M/main_s.o \
./Drivers/CMSIS/Core/Template/ARMv8-M/tz_context.o 

C_DEPS += \
./Drivers/CMSIS/Core/Template/ARMv8-M/main_s.d \
./Drivers/CMSIS/Core/Template/ARMv8-M/tz_context.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Core/Template/ARMv8-M/%.o: ../Drivers/CMSIS/Core/Template/ARMv8-M/%.c Drivers/CMSIS/Core/Template/ARMv8-M/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-Core-2f-Template-2f-ARMv8-2d-M

clean-Drivers-2f-CMSIS-2f-Core-2f-Template-2f-ARMv8-2d-M:
	-$(RM) ./Drivers/CMSIS/Core/Template/ARMv8-M/main_s.d ./Drivers/CMSIS/Core/Template/ARMv8-M/main_s.o ./Drivers/CMSIS/Core/Template/ARMv8-M/tz_context.d ./Drivers/CMSIS/Core/Template/ARMv8-M/tz_context.o

.PHONY: clean-Drivers-2f-CMSIS-2f-Core-2f-Template-2f-ARMv8-2d-M

