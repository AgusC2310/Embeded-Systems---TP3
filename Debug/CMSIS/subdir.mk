################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MK64F12.c 

C_DEPS += \
./CMSIS/system_MK64F12.d 

OBJS += \
./CMSIS/system_MK64F12.o 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c CMSIS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VDC12 -DCPU_MK64FN1M0VDC12_cm4 -D__REDLIB__ -I"C:\Users\casas\OneDrive - ITBA\Material de Clases\Labo de Micros\ADC_DAC_code samples\ADC\source" -I"C:\Users\casas\OneDrive - ITBA\Material de Clases\Labo de Micros\ADC_DAC_code samples\ADC" -I"C:\Users\casas\OneDrive - ITBA\Material de Clases\Labo de Micros\ADC_DAC_code samples\ADC\startup" -I"C:\Users\casas\OneDrive - ITBA\Material de Clases\Labo de Micros\ADC_DAC_code samples\ADC\CMSIS" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wa,-adhlns="$@.lst" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-CMSIS

clean-CMSIS:
	-$(RM) ./CMSIS/system_MK64F12.d ./CMSIS/system_MK64F12.o

.PHONY: clean-CMSIS

