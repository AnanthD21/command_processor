################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/cbfifo.c \
../source/cmd_handler.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sys_read_write.c \
../source/sysclock.c \
../source/test_cbfifo.c \
../source/uart.c 

OBJS += \
./source/cbfifo.o \
./source/cmd_handler.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sys_read_write.o \
./source/sysclock.o \
./source/test_cbfifo.o \
./source/uart.o 

C_DEPS += \
./source/cbfifo.d \
./source/cmd_handler.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sys_read_write.d \
./source/sysclock.d \
./source/test_cbfifo.d \
./source/uart.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\Ananth Deshpande\Documents\MCUXpressoIDE_11.2.0_4120\workspace\BreakfastSerial\source" -I"C:\Users\Ananth Deshpande\Documents\MCUXpressoIDE_11.2.0_4120\workspace\BreakfastSerial" -I"C:\Users\Ananth Deshpande\Documents\MCUXpressoIDE_11.2.0_4120\workspace\BreakfastSerial\startup" -I"C:\Users\Ananth Deshpande\Documents\MCUXpressoIDE_11.2.0_4120\workspace\BreakfastSerial\CMSIS" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


