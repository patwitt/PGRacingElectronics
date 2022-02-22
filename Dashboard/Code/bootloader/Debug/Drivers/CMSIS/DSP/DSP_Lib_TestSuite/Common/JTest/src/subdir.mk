################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_cycle.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_dump_str_segments.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_fw.c \
../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_trigger_action.c 

OBJS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_cycle.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_dump_str_segments.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_fw.o \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_trigger_action.o 

C_DEPS += \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_cycle.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_dump_str_segments.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_fw.d \
./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_trigger_action.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/%.o: ../Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/%.c Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-JTest-2f-src

clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-JTest-2f-src:
	-$(RM) ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_cycle.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_cycle.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_dump_str_segments.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_dump_str_segments.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_fw.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_fw.o ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_trigger_action.d ./Drivers/CMSIS/DSP/DSP_Lib_TestSuite/Common/JTest/src/jtest_trigger_action.o

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-DSP_Lib_TestSuite-2f-Common-2f-JTest-2f-src

