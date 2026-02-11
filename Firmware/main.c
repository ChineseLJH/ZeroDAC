#include "pico/stdlib.h"
#include "ws2812_driver.h" // 引用你的驱动头文件

// RP2040-Zero 板载灯是 GP16
#define PIN_LED 16

int main() {
    stdio_init_all();

    // 1. 初始化驱动 (使用 PIO0, GP16)
    ws2812_driver_init(pio0, PIN_LED);

    while (1) {
        // 红色
        ws2812_put_pixel(pio0, 20, 0, 0); 
        sleep_ms(500);

        // 绿色
        ws2812_put_pixel(pio0, 0, 20, 0);
        sleep_ms(500);

        // 蓝色
        ws2812_put_pixel(pio0, 0, 0, 20);
        sleep_ms(500);
    }
}