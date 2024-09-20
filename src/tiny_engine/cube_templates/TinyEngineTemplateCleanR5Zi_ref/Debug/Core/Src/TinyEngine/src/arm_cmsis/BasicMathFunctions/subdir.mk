################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_add_s8.c \
../Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_mul_s8.c 

OBJS += \
./Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_add_s8.o \
./Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_mul_s8.o 

C_DEPS += \
./Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_add_s8.d \
./Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_mul_s8.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/%.o Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/%.su: ../Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/%.c Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4R5xx -c -I../Core/Inc -I../Core/Src/TinyEngine/include/arm_cmsis -I../Core/Src/TinyEngine/include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-TinyEngine-2f-src-2f-arm_cmsis-2f-BasicMathFunctions

clean-Core-2f-Src-2f-TinyEngine-2f-src-2f-arm_cmsis-2f-BasicMathFunctions:
	-$(RM) ./Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_add_s8.d ./Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_add_s8.o ./Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_add_s8.su ./Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_mul_s8.d ./Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_mul_s8.o ./Core/Src/TinyEngine/src/arm_cmsis/BasicMathFunctions/arm_elementwise_mul_s8.su

.PHONY: clean-Core-2f-Src-2f-TinyEngine-2f-src-2f-arm_cmsis-2f-BasicMathFunctions

