################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/abs.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/add.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/dot_prod.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/mult.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/negate.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/offset.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/scale.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/shift.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/sub.c 

OBJS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/abs.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/add.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/dot_prod.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/mult.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/negate.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/offset.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/scale.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/shift.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/sub.o 

C_DEPS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/abs.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/add.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/dot_prod.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/mult.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/negate.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/offset.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/scale.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/shift.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/sub.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/%.o: ../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/%.c Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-RefLibs-2f-src-2f-BasicMathFunctions

clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-RefLibs-2f-src-2f-BasicMathFunctions:
	-$(RM) ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/abs.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/abs.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/add.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/add.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/dot_prod.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/dot_prod.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/mult.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/mult.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/negate.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/negate.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/offset.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/offset.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/scale.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/scale.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/shift.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/shift.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/sub.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/RefLibs/src/BasicMathFunctions/sub.o

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-RefLibs-2f-src-2f-BasicMathFunctions

