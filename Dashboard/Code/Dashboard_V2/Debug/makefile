################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Utilities/JPEG/subdir.mk
-include TouchGFX/target/generated/subdir.mk
-include TouchGFX/target/subdir.mk
-include TouchGFX/gui/src/model/subdir.mk
-include TouchGFX/gui/src/main_screen/subdir.mk
-include TouchGFX/gui/src/lana_screen/subdir.mk
-include TouchGFX/gui/src/debuginfo_screen/subdir.mk
-include TouchGFX/gui/src/containers/subdir.mk
-include TouchGFX/gui/src/common/subdir.mk
-include TouchGFX/generated/texts/src/subdir.mk
-include TouchGFX/generated/images/src/subdir.mk
-include TouchGFX/generated/gui_generated/src/main_screen/subdir.mk
-include TouchGFX/generated/gui_generated/src/lana_screen/subdir.mk
-include TouchGFX/generated/gui_generated/src/debuginfo_screen/subdir.mk
-include TouchGFX/generated/gui_generated/src/containers/subdir.mk
-include TouchGFX/generated/gui_generated/src/common/subdir.mk
-include TouchGFX/generated/fonts/src/subdir.mk
-include TouchGFX/App/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/Inc/subdir.mk
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


BUILD_ARTIFACT_NAME := Dashboard_V2
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Dashboard_V2.elf \

MAP_FILES += \
Dashboard_V2.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Dashboard_V2.list \


# All Target
all:
	+@$(MAKE) --no-print-directory pre-build && $(MAKE) --no-print-directory main-build

# Main-build Target
main-build: Dashboard_V2.elf secondary-outputs

# Tool invocations
Dashboard_V2.elf Dashboard_V2.map: $(OBJS) $(USER_OBJS) F:\Marcin\Documents\Studia\PGRacing\PGRacingElectronics\Dashboard\Code\Dashboard_V2\STM32F767ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "Dashboard_V2.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"F:\Marcin\Documents\Studia\PGRacing\PGRacingElectronics\Dashboard\Code\Dashboard_V2\STM32F767ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Dashboard_V2.map" -Wl,--gc-sections -static -L"F:\Marcin\Documents\Studia\PGRacing\PGRacingElectronics\Dashboard\Code\Dashboard_V2\Middlewares\ST\touchgfx\lib\core\cortex_m7\gcc" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Dashboard_V2.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Dashboard_V2.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Dashboard_V2.elf Dashboard_V2.list Dashboard_V2.map default.size.stdout
	-@echo ' '

pre-build:
	powershell.exe "..\git2header.ps1"
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified pre-build

-include ../makefile.targets
