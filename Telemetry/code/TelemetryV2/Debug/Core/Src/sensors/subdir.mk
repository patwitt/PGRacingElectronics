################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/sensors/ABS.c \
../Core/Src/sensors/ADCSensor.c \
../Core/Src/sensors/GPS.c \
../Core/Src/sensors/IMU.c \
../Core/Src/sensors/MLX.c \
../Core/Src/sensors/MLX90640_API.c \
../Core/Src/sensors/MLX90640_I2C_Driver.c \
../Core/Src/sensors/MPU9250.c \
../Core/Src/sensors/TeleBack.c 

OBJS += \
./Core/Src/sensors/ABS.o \
./Core/Src/sensors/ADCSensor.o \
./Core/Src/sensors/GPS.o \
./Core/Src/sensors/IMU.o \
./Core/Src/sensors/MLX.o \
./Core/Src/sensors/MLX90640_API.o \
./Core/Src/sensors/MLX90640_I2C_Driver.o \
./Core/Src/sensors/MPU9250.o \
./Core/Src/sensors/TeleBack.o 

C_DEPS += \
./Core/Src/sensors/ABS.d \
./Core/Src/sensors/ADCSensor.d \
./Core/Src/sensors/GPS.d \
./Core/Src/sensors/IMU.d \
./Core/Src/sensors/MLX.d \
./Core/Src/sensors/MLX90640_API.d \
./Core/Src/sensors/MLX90640_I2C_Driver.d \
./Core/Src/sensors/MPU9250.d \
./Core/Src/sensors/TeleBack.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/sensors/%.o Core/Src/sensors/%.su Core/Src/sensors/%.cyclo: ../Core/Src/sensors/%.c Core/Src/sensors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-sensors

clean-Core-2f-Src-2f-sensors:
	-$(RM) ./Core/Src/sensors/ABS.cyclo ./Core/Src/sensors/ABS.d ./Core/Src/sensors/ABS.o ./Core/Src/sensors/ABS.su ./Core/Src/sensors/ADCSensor.cyclo ./Core/Src/sensors/ADCSensor.d ./Core/Src/sensors/ADCSensor.o ./Core/Src/sensors/ADCSensor.su ./Core/Src/sensors/GPS.cyclo ./Core/Src/sensors/GPS.d ./Core/Src/sensors/GPS.o ./Core/Src/sensors/GPS.su ./Core/Src/sensors/IMU.cyclo ./Core/Src/sensors/IMU.d ./Core/Src/sensors/IMU.o ./Core/Src/sensors/IMU.su ./Core/Src/sensors/MLX.cyclo ./Core/Src/sensors/MLX.d ./Core/Src/sensors/MLX.o ./Core/Src/sensors/MLX.su ./Core/Src/sensors/MLX90640_API.cyclo ./Core/Src/sensors/MLX90640_API.d ./Core/Src/sensors/MLX90640_API.o ./Core/Src/sensors/MLX90640_API.su ./Core/Src/sensors/MLX90640_I2C_Driver.cyclo ./Core/Src/sensors/MLX90640_I2C_Driver.d ./Core/Src/sensors/MLX90640_I2C_Driver.o ./Core/Src/sensors/MLX90640_I2C_Driver.su ./Core/Src/sensors/MPU9250.cyclo ./Core/Src/sensors/MPU9250.d ./Core/Src/sensors/MPU9250.o ./Core/Src/sensors/MPU9250.su ./Core/Src/sensors/TeleBack.cyclo ./Core/Src/sensors/TeleBack.d ./Core/Src/sensors/TeleBack.o ./Core/Src/sensors/TeleBack.su

.PHONY: clean-Core-2f-Src-2f-sensors

