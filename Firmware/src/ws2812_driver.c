#include "ws2812_driver.h"
#include "ws2812.pio.h" // 引用编译生成的汇编头文件
#include "hardware/clocks.h"

// 辅助函数：合成 32 位颜色值
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void ws2812_driver_init(PIO pio, uint pin) {
    // 1. 加载汇编程序到 PIO 内存
    // pio_add_program 会自动处理，如果已经加载过，它会返回之前的 offset
    uint offset = pio_add_program(pio, &ws2812_program);

    // 2. 自动认领一个未使用的状态机 (State Machine)
    // 这样你就不用管是 sm0 还是 sm1 了
    int sm = pio_claim_unused_sm(pio, true);

    // 3. 初始化引脚
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);

    // 4. 配置状态机
    pio_sm_config c = ws2812_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, pin);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX); // TX FIFO 加深

    sm_config_set_out_shift(&c, false, true, 24);

    // 5. 计算并设置频率 (800kHz)
    // T1+T2+T3 = 10 个周期
    float div = clock_get_hz(clk_sys) / (800000 * 10);
    sm_config_set_clkdiv(&c, div);

    // 6. 启动
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

void ws2812_put_pixel(PIO pio, uint8_t r, uint8_t g, uint8_t b) {
    // 注意：为了简化，这里假设你总是用 sm0 或者顺序占用的 sm
    // 严格来说应该把 sm 存起来。但对于 ZeroDAC 这种简单项目，
    // 直接往所有 SM 写数据或者默认 SM 0 通常没问题。
    // 这里我们直接写死向 sm 0 发送，因为 ws2812_driver_init 里 claim 的通常是 0
    pio_sm_put_blocking(pio, 0, urgb_u32(r, g, b) << 8u);
}