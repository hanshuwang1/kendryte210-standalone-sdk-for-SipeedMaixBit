#include "timer.h"
#include "sysctl.h"
#include "plic.h"
#include <stdio.h>
#include "interrupt.h"
#include "lcd.h"

int timer0_timeout_cb(void *ctx);

void gtimer_init(void)
{
    /* 初始化定时器 */
    timer_init(TIMER_DEVICE_0);
    /* 设置定时器周期 */
    timer_set_interval(TIMER_DEVICE_0, TIMER_CHANNEL_0, 500000000);
    /* 设置定时器中断回调 */
    timer_irq_register(TIMER_DEVICE_0, TIMER_CHANNEL_0, 0, 1, timer0_timeout_cb, NULL);
    /* 使能定时器中断 */
    timer_set_enable(TIMER_DEVICE_0, TIMER_CHANNEL_0, 1);
}

int timer0_timeout_cb(void *ctx)
{
    printf("rx_buff: %d\n", rx_buff[0]);
    return 0;
}