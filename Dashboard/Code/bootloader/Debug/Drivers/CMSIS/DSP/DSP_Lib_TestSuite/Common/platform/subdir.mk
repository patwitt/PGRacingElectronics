################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM0.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM23.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM3.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM33.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM4.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM7.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC000.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC300.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MBL.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MML.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_generic.c 

S_UPPER_SRCS += \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/startup_generic.S 

OBJS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/startup_generic.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM0.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM23.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM3.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM33.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM4.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM7.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC000.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC300.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MBL.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MML.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_generic.o 

S_UPPER_DEPS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/startup_generic.d 

C_DEPS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM0.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM23.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM3.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM33.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM4.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM7.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC000.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC300.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MBL.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MML.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_generic.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/%.o: ../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/%.S Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/%.o: ../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/%.c Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-platform

clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-platform:
	-$(RM) ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/startup_generic.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/startup_generic.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM0.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM0.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM23.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM23.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM3.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM3.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM33.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM33.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM4.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM4.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM7.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM7.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC000.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC000.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC300.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC300.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MBL.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MBL.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MML.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MML.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_generic.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/platform/system_generic.o

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-platform

