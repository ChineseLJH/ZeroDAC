#ifndef WS2812_DRIVER_H
#define WS2812_DRIVER_H

#include "pico/stdlib.h"
#include "hardware/pio.h"

// 初始化函数：传入 PIO 模块和引脚号
void ws2812_driver_init(PIO pio, uint pin);

// 发送颜色函数：传入 PIO 模块和 RGB 值
void ws2812_put_pixel(PIO pio, uint8_t r, uint8_t g, uint8_t b);

#endif