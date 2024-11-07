################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/ADC.c \
../source/FTM.c \
../source/GPIO.c \
../source/PORT.c \
../source/SysTick.c \
../source/demod.c \
../source/hardware.c \
../source/main.c \
../source/uart2.c 

C_DEPS += \
./source/ADC.d \
./source/FTM.d \
./source/GPIO.d \
./source/PORT.d \
./source/SysTick.d \
./source/demod.d \
./source/hardware.d \
./source/main.d \
./source/uart2.d 

OBJS += \
./source/ADC.o \
./source/FTM.o \
./source/GPIO.o \
./source/PORT.o \
./source/SysTick.o \
./source/demod.o \
./source/hardware.o \
./source/main.o \
./source/uart2.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VDC12 -DCPU_MK64FN1M0VDC12_cm4 -D__REDLIB__ -I"C:\Users\casas\OneDrive - ITBA\Material de Clases\Labo de Micros\ADC_DAC_code samples\ADC\source" -I"C:\Users\casas\OneDrive - ITBA\Material de Clases\Labo de Micros\ADC_DAC_code samples\ADC" -I"C:\Users\casas\OneDrive - ITBA\Material de Clases\Labo de Micros\ADC_DAC_code samples\ADC\startup" -I"C:\Users\casas\OneDrive - ITBA\Material de Clases\Labo de Micros\ADC_DAC_code samples\ADC\CMSIS" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wa,-adhlns="$@.lst" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/ADC.d ./source/ADC.o ./source/FTM.d ./source/FTM.o ./source/GPIO.d ./source/GPIO.o ./source/PORT.d ./source/PORT.o ./source/SysTick.d ./source/SysTick.o ./source/demod.d ./source/demod.o ./source/hardware.d ./source/hardware.o ./source/main.d ./source/main.o ./source/uart2.d ./source/uart2.o

.PHONY: clean-source

