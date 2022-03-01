################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/copy.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fill.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fixed_to_fixed.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fixed_to_float.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/float_to_fixed.c 

OBJS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/copy.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fill.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fixed_to_fixed.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fixed_to_float.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/float_to_fixed.o 

C_DEPS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/copy.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fill.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fixed_to_fixed.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fixed_to_float.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/float_to_fixed.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/%.o: ../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/%.c Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-RefLibs-2f-src-2f-SupportFunctions

clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-RefLibs-2f-src-2f-SupportFunctions:
	-$(RM) ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/copy.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/copy.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fill.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fill.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fixed_to_fixed.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fixed_to_fixed.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fixed_to_float.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/fixed_to_float.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/float_to_fixed.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/SupportFunctions/float_to_fixed.o

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-RefLibs-2f-src-2f-SupportFunctions

