################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.c \
../Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.c \
../Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15.c \
../Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.c \
../Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7.c \
../Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c 

OBJS += \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.o \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.o \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15.o \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.o \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7.o \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.o 

C_DEPS += \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.d \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.d \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15.d \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.d \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7.d \
./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/NN/Source/FullyConnectedFunctions/%.o: ../Drivers/CMSIS/NN/Source/FullyConnectedFunctions/%.c Drivers/CMSIS/NN/Source/FullyConnectedFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-NN-2f-Source-2f-FullyConnectedFunctions

clean-Drivers-2f-CMSIS-2f-NN-2f-Source-2f-FullyConnectedFunctions:
	-$(RM) ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.d ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.o ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.d ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.o ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15.d ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15.o ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.d ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.o ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7.d ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7.o ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.d ./Drivers/CMSIS/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.o

.PHONY: clean-Drivers-2f-CMSIS-2f-NN-2f-Source-2f-FullyConnectedFunctions

