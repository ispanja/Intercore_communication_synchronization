################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/main.c \
../src/smphr.c \
../src/sys_init.c 

OBJS += \
./src/main.o \
./src/smphr.o \
./src/sys_init.o 

C_DEPS += \
./src/main.d \
./src/smphr.d \
./src/sys_init.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM A53 gcc compiler'
	aarch64-none-elf-gcc -Wall -O0 -g3 -I/home/ivan/libmetal/build_libmetal/lib/include -c -fmessage-length=0 -MT"$@" -I../../peterson_ultrazed1_bsp/psu_cortexa53_1/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


