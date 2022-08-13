################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/MLX90640_API.c \
../Core/Src/MLX90640_I2C_Driver.c \
../Core/Src/MPU9250.c \
../Core/Src/SDCARD.c \
../Core/Src/adc.c \
../Core/Src/basicFunctions.c \
../Core/Src/can.c \
../Core/Src/dma.c \
../Core/Src/ecumaster.c \
../Core/Src/gpio.c \
../Core/Src/handler.c \
../Core/Src/i2c.c \
../Core/Src/main.c \
../Core/Src/queue.c \
../Core/Src/rtc.c \
../Core/Src/sdmmc.c \
../Core/Src/sensorFunctions.c \
../Core/Src/stm32f7xx_hal_msp.c \
../Core/Src/stm32f7xx_it.c \
../Core/Src/system_stm32f7xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/MLX90640_API.o \
./Core/Src/MLX90640_I2C_Driver.o \
./Core/Src/MPU9250.o \
./Core/Src/SDCARD.o \
./Core/Src/adc.o \
./Core/Src/basicFunctions.o \
./Core/Src/can.o \
./Core/Src/dma.o \
./Core/Src/ecumaster.o \
./Core/Src/gpio.o \
./Core/Src/handler.o \
./Core/Src/i2c.o \
./Core/Src/main.o \
./Core/Src/queue.o \
./Core/Src/rtc.o \
./Core/Src/sdmmc.o \
./Core/Src/sensorFunctions.o \
./Core/Src/stm32f7xx_hal_msp.o \
./Core/Src/stm32f7xx_it.o \
./Core/Src/system_stm32f7xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/MLX90640_API.d \
./Core/Src/MLX90640_I2C_Driver.d \
./Core/Src/MPU9250.d \
./Core/Src/SDCARD.d \
./Core/Src/adc.d \
./Core/Src/basicFunctions.d \
./Core/Src/can.d \
./Core/Src/dma.d \
./Core/Src/ecumaster.d \
./Core/Src/gpio.d \
./Core/Src/handler.d \
./Core/Src/i2c.d \
./Core/Src/main.d \
./Core/Src/queue.d \
./Core/Src/rtc.d \
./Core/Src/sdmmc.d \
./Core/Src/sensorFunctions.d \
./Core/Src/stm32f7xx_hal_msp.d \
./Core/Src/stm32f7xx_it.d \
./Core/Src/system_stm32f7xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -c -I../Core/Inc -I../FATFS/Target -I../FATFS/App -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/MLX90640_API.d ./Core/Src/MLX90640_API.o ./Core/Src/MLX90640_API.su ./Core/Src/MLX90640_I2C_Driver.d ./Core/Src/MLX90640_I2C_Driver.o ./Core/Src/MLX90640_I2C_Driver.su ./Core/Src/MPU9250.d ./Core/Src/MPU9250.o ./Core/Src/MPU9250.su ./Core/Src/SDCARD.d ./Core/Src/SDCARD.o ./Core/Src/SDCARD.su ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/basicFunctions.d ./Core/Src/basicFunctions.o ./Core/Src/basicFunctions.su ./Core/Src/can.d ./Core/Src/can.o ./Core/Src/can.su ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/ecumaster.d ./Core/Src/ecumaster.o ./Core/Src/ecumaster.su ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/handler.d ./Core/Src/handler.o ./Core/Src/handler.su ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/queue.d ./Core/Src/queue.o ./Core/Src/queue.su ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/sdmmc.d ./Core/Src/sdmmc.o ./Core/Src/sdmmc.su ./Core/Src/sensorFunctions.d ./Core/Src/sensorFunctions.o ./Core/Src/sensorFunctions.su ./Core/Src/stm32f7xx_hal_msp.d ./Core/Src/stm32f7xx_hal_msp.o ./Core/Src/stm32f7xx_hal_msp.su ./Core/Src/stm32f7xx_it.d ./Core/Src/stm32f7xx_it.o ./Core/Src/stm32f7xx_it.su ./Core/Src/system_stm32f7xx.d ./Core/Src/system_stm32f7xx.o ./Core/Src/system_stm32f7xx.su ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

