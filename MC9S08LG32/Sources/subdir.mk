################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/CRC.c" \
"../Sources/Cpu.c" \
"../Sources/DS1804.c" \
"../Sources/Keyboard.c" \
"../Sources/LCD.c" \
"../Sources/P_inputs.c" \
"../Sources/P_outputs.c" \
"../Sources/Vectors.c" \
"../Sources/eeprom.c" \
"../Sources/main.c" \
"../Sources/onems.c" \
"../Sources/print.c" \
"../Sources/state.c" \
"../Sources/string.c" \
"../Sources/testcode.c" \
"../Sources/util.c" \
"../Sources/weldhold.c" \

C_SRCS += \
../Sources/CRC.c \
../Sources/Cpu.c \
../Sources/DS1804.c \
../Sources/Keyboard.c \
../Sources/LCD.c \
../Sources/P_inputs.c \
../Sources/P_outputs.c \
../Sources/Vectors.c \
../Sources/eeprom.c \
../Sources/main.c \
../Sources/onems.c \
../Sources/print.c \
../Sources/state.c \
../Sources/string.c \
../Sources/testcode.c \
../Sources/util.c \
../Sources/weldhold.c \

OBJS += \
./Sources/CRC_c.obj \
./Sources/Cpu_c.obj \
./Sources/DS1804_c.obj \
./Sources/Keyboard_c.obj \
./Sources/LCD_c.obj \
./Sources/P_inputs_c.obj \
./Sources/P_outputs_c.obj \
./Sources/Vectors_c.obj \
./Sources/eeprom_c.obj \
./Sources/main_c.obj \
./Sources/onems_c.obj \
./Sources/print_c.obj \
./Sources/state_c.obj \
./Sources/string_c.obj \
./Sources/testcode_c.obj \
./Sources/util_c.obj \
./Sources/weldhold_c.obj \

OBJS_QUOTED += \
"./Sources/CRC_c.obj" \
"./Sources/Cpu_c.obj" \
"./Sources/DS1804_c.obj" \
"./Sources/Keyboard_c.obj" \
"./Sources/LCD_c.obj" \
"./Sources/P_inputs_c.obj" \
"./Sources/P_outputs_c.obj" \
"./Sources/Vectors_c.obj" \
"./Sources/eeprom_c.obj" \
"./Sources/main_c.obj" \
"./Sources/onems_c.obj" \
"./Sources/print_c.obj" \
"./Sources/state_c.obj" \
"./Sources/string_c.obj" \
"./Sources/testcode_c.obj" \
"./Sources/util_c.obj" \
"./Sources/weldhold_c.obj" \

C_DEPS += \
./Sources/CRC_c.d \
./Sources/Cpu_c.d \
./Sources/DS1804_c.d \
./Sources/Keyboard_c.d \
./Sources/LCD_c.d \
./Sources/P_inputs_c.d \
./Sources/P_outputs_c.d \
./Sources/Vectors_c.d \
./Sources/eeprom_c.d \
./Sources/main_c.d \
./Sources/onems_c.d \
./Sources/print_c.d \
./Sources/state_c.d \
./Sources/string_c.d \
./Sources/testcode_c.d \
./Sources/util_c.d \
./Sources/weldhold_c.d \

OBJS_OS_FORMAT += \
./Sources/CRC_c.obj \
./Sources/Cpu_c.obj \
./Sources/DS1804_c.obj \
./Sources/Keyboard_c.obj \
./Sources/LCD_c.obj \
./Sources/P_inputs_c.obj \
./Sources/P_outputs_c.obj \
./Sources/Vectors_c.obj \
./Sources/eeprom_c.obj \
./Sources/main_c.obj \
./Sources/onems_c.obj \
./Sources/print_c.obj \
./Sources/state_c.obj \
./Sources/string_c.obj \
./Sources/testcode_c.obj \
./Sources/util_c.obj \
./Sources/weldhold_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/CRC_c.obj: ../Sources/CRC.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/CRC.args" -ObjN="Sources/CRC_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Cpu_c.obj: ../Sources/Cpu.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/Cpu.args" -ObjN="Sources/Cpu_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/DS1804_c.obj: ../Sources/DS1804.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/DS1804.args" -ObjN="Sources/DS1804_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/Keyboard_c.obj: ../Sources/Keyboard.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/Keyboard.args" -ObjN="Sources/Keyboard_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/LCD_c.obj: ../Sources/LCD.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/LCD.args" -ObjN="Sources/LCD_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/P_inputs_c.obj: ../Sources/P_inputs.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/P_inputs.args" -ObjN="Sources/P_inputs_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/P_outputs_c.obj: ../Sources/P_outputs.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/P_outputs.args" -ObjN="Sources/P_outputs_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/Vectors_c.obj: ../Sources/Vectors.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/Vectors.args" -ObjN="Sources/Vectors_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/eeprom_c.obj: ../Sources/eeprom.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/eeprom.args" -ObjN="Sources/eeprom_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/main.args" -ObjN="Sources/main_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/onems_c.obj: ../Sources/onems.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/onems.args" -ObjN="Sources/onems_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/print_c.obj: ../Sources/print.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/print.args" -ObjN="Sources/print_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/state_c.obj: ../Sources/state.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/state.args" -ObjN="Sources/state_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/string_c.obj: ../Sources/string.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/string.args" -ObjN="Sources/string_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/testcode_c.obj: ../Sources/testcode.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/testcode.args" -ObjN="Sources/testcode_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/util_c.obj: ../Sources/util.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/util.args" -ObjN="Sources/util_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/weldhold_c.obj: ../Sources/weldhold.c
	@echo 'Building file: $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/weldhold.args" -ObjN="Sources/weldhold_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


