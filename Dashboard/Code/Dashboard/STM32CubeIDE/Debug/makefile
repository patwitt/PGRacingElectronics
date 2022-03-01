################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/FreeRTOS/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Application/User/gui/subdir.mk
-include Application/User/generated/subdir.mk
-include Application/User/TouchGFX/target/generated/subdir.mk
-include Application/User/TouchGFX/target/subdir.mk
-include Application/User/TouchGFX/App/subdir.mk
-include Application/User/Startup/subdir.mk
-include Application/User/Core/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := code
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
code.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
code.list \

OBJCOPY_BIN += \
code.bin \


# All Target
all: main-build

# Main-build Target
main-build: code.elf secondary-outputs

# Tool invocations
code.elf: $(OBJS) $(USER_OBJS) F:\Marcin\Documents\Studia\PGRacing\PGRacingElectronics\Dashboard\Code\Dashboard\STM32CubeIDE\STM32F767ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "code.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"F:\Marcin\Documents\Studia\PGRacing\PGRacingElectronics\Dashboard\Code\Dashboard\STM32CubeIDE\STM32F767ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="code.map" -Wl,--gc-sections -static -LF:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/code/Middlewares/ST/touchgfx/lib/core/cortex_m7/gcc -LF:/Marcin/Documents/Studia/PGRacing/PGRacingElectronics/Dashboard/Code/Dashboard/Middlewares/ST/touchgfx/lib/core/cortex_m7/gcc --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

code.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "code.list"
	@echo 'Finished building: $@'
	@echo ' '

code.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "code.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) code.bin code.elf code.list default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
