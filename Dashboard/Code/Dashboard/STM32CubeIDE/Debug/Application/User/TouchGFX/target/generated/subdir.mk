################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/target/generated/OSWrappers.cpp \
F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/target/generated/TouchGFXConfiguration.cpp \
F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/target/generated/TouchGFXGeneratedHAL.cpp 

OBJS += \
./Application/User/TouchGFX/target/generated/OSWrappers.o \
./Application/User/TouchGFX/target/generated/TouchGFXConfiguration.o \
./Application/User/TouchGFX/target/generated/TouchGFXGeneratedHAL.o 

CPP_DEPS += \
./Application/User/TouchGFX/target/generated/OSWrappers.d \
./Application/User/TouchGFX/target/generated/TouchGFXConfiguration.d \
./Application/User/TouchGFX/target/generated/TouchGFXGeneratedHAL.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/TouchGFX/target/generated/OSWrappers.o: F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/target/generated/OSWrappers.cpp Application/User/TouchGFX/target/generated/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/TouchGFX/target/generated/TouchGFXConfiguration.o: F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/target/generated/TouchGFXConfiguration.cpp Application/User/TouchGFX/target/generated/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/TouchGFX/target/generated/TouchGFXGeneratedHAL.o: F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/target/generated/TouchGFXGeneratedHAL.cpp Application/User/TouchGFX/target/generated/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-TouchGFX-2f-target-2f-generated

clean-Application-2f-User-2f-TouchGFX-2f-target-2f-generated:
	-$(RM) ./Application/User/TouchGFX/target/generated/OSWrappers.d ./Application/User/TouchGFX/target/generated/OSWrappers.o ./Application/User/TouchGFX/target/generated/TouchGFXConfiguration.d ./Application/User/TouchGFX/target/generated/TouchGFXConfiguration.o ./Application/User/TouchGFX/target/generated/TouchGFXGeneratedHAL.d ./Application/User/TouchGFX/target/generated/TouchGFXGeneratedHAL.o

.PHONY: clean-Application-2f-User-2f-TouchGFX-2f-target-2f-generated

