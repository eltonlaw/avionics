# For reference only, this is what STM32CubeIDE does when building the project
arm-none-eabi-gcc "../Core/Src/main.c" -mcpu=cortex-m0plus -std=gnu11 -g3 \
  -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc \
  -I/home/d0nkrs/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1/Drivers/STM32G0xx_HAL_Driver/Inc \
  -I/home/d0nkrs/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy \
  -I/home/d0nkrs/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1/Drivers/CMSIS/Device/ST/STM32G0xx/Include \
  -I/home/d0nkrs/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1/Drivers/CMSIS/Include \
  -I/home/d0nkrs/dev/robotics/stlib/Inc -O0 -ffunction-sections -fdata-sections \
  -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Src/main.d" \
  -MT"Core/Src/main.o" --specs=nano.specs -mfloat-abi=soft -mthumb -o \
  "Core/Src/main.o"

arm-none-eabi-gcc -o "fc2.elf" @"objects.list"   -mcpu=cortex-m0plus \
  -T"/home/d0nkrs/dev/robotics/fc2/STM32G070RBTX_FLASH.ld" --specs=nosys.specs \
  -Wl,-Map="fc2.map" -Wl,--gc-sections -static -u _printf_float \
  --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm \
  -Wl,--end-group
