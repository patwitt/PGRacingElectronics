################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Application/User/Startup/startup_stm32f767zitx.s 

S_DEPS += \
./Application/User/Startup/startup_stm32f767zitx.d 

OBJS += \
./Application/User/Startup/startup_stm32f767zitx.o 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Startup/%.o: ../Application/User/Startup/%.s Application/User/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/Code/Dashboard/STM32CubeIDE/Application/User/Core" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Application-2f-User-2f-Startup

clean-Application-2f-User-2f-Startup:
	-$(RM) ./Application/User/Startup/startup_stm32f767zitx.d ./Application/User/Startup/startup_stm32f767zitx.o

.PHONY: clean-Application-2f-User-2f-Startup

