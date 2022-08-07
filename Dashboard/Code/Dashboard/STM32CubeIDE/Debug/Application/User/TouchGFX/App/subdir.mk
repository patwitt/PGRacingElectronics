################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/Code/Dashboard/TouchGFX/App/app_touchgfx.c 

OBJS += \
./Application/User/TouchGFX/App/app_touchgfx.o 

C_DEPS += \
./Application/User/TouchGFX/App/app_touchgfx.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/TouchGFX/App/app_touchgfx.o: F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/Code/Dashboard/TouchGFX/App/app_touchgfx.c Application/User/TouchGFX/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I"F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/Code/Dashboard/STM32CubeIDE/Application/User/Core" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-TouchGFX-2f-App

clean-Application-2f-User-2f-TouchGFX-2f-App:
	-$(RM) ./Application/User/TouchGFX/App/app_touchgfx.d ./Application/User/TouchGFX/App/app_touchgfx.o

.PHONY: clean-Application-2f-User-2f-TouchGFX-2f-App

