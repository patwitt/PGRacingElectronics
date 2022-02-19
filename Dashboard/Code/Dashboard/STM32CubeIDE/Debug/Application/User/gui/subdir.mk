################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/common/FrontendApplication.cpp \
F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/model/Model.cpp \
F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/main_screen/mainPresenter.cpp \
F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/main_screen/mainView.cpp \
F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/start_screen/startPresenter.cpp \
F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/start_screen/startView.cpp 

OBJS += \
./Application/User/gui/FrontendApplication.o \
./Application/User/gui/Model.o \
./Application/User/gui/mainPresenter.o \
./Application/User/gui/mainView.o \
./Application/User/gui/startPresenter.o \
./Application/User/gui/startView.o 

CPP_DEPS += \
./Application/User/gui/FrontendApplication.d \
./Application/User/gui/Model.d \
./Application/User/gui/mainPresenter.d \
./Application/User/gui/mainView.d \
./Application/User/gui/startPresenter.d \
./Application/User/gui/startView.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/gui/FrontendApplication.o: F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/common/FrontendApplication.cpp Application/User/gui/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/gui/Model.o: F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/model/Model.cpp Application/User/gui/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/gui/mainPresenter.o: F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/main_screen/mainPresenter.cpp Application/User/gui/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/gui/mainView.o: F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/main_screen/mainView.cpp Application/User/gui/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/gui/startPresenter.o: F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/start_screen/startPresenter.cpp Application/User/gui/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/gui/startView.o: F:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/TouchGFX/gui/src/start_screen/startView.cpp Application/User/gui/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../../Core/Inc -I../../TouchGFX/App -I../../TouchGFX/target/generated -I../../TouchGFX/target -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-gui

clean-Application-2f-User-2f-gui:
	-$(RM) ./Application/User/gui/FrontendApplication.d ./Application/User/gui/FrontendApplication.o ./Application/User/gui/Model.d ./Application/User/gui/Model.o ./Application/User/gui/mainPresenter.d ./Application/User/gui/mainPresenter.o ./Application/User/gui/mainView.d ./Application/User/gui/mainView.o ./Application/User/gui/startPresenter.d ./Application/User/gui/startPresenter.o ./Application/User/gui/startView.d ./Application/User/gui/startView.o

.PHONY: clean-Application-2f-User-2f-gui

