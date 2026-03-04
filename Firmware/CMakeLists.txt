cmake_minimum_required(VERSION 3.13)

# 引入 SDK
include(pico_sdk_import.cmake)

# 定义项目名称
project(ZeroDAC_Firmware C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# 初始化 SDK
pico_sdk_init()

include_directories(include)

# 定义可执行文件
add_executable(firmware
    main.c
    src/ws2812_driver.c
)

# 关键：生成 PIO 头文件 (会生成 ws2812.pio.h)
pico_generate_pio_header(firmware ${CMAKE_CURRENT_LIST_DIR}/src/ws2812.pio)

# 链接库
target_link_libraries(firmware
    pico_stdlib
    hardware_pio
)

# 启用 USB 输出 (可选，方便后续 printf 调试)
pico_enable_stdio_usb(firmware 1)
pico_enable_stdio_uart(firmware 0)

# 生成 .uf2 文件
pico_add_extra_outputs(firmware)

if(DEFINED ENV{USER} AND NOT "$ENV{USER}" STREQUAL "")
    set(RP2040_MOUNT_DIR "/media/$ENV{USER}/RPI-RP2")
else()
    set(RP2040_MOUNT_DIR "/media/RPI-RP2")
endif()

add_custom_target(flash
    DEPENDS firmware
    # 自动读取当前用户名拼接挂载点，避免硬编码用户名
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/firmware.uf2 ${RP2040_MOUNT_DIR}/
    COMMENT "Flashing firmware.uf2 to RP2040..."
)