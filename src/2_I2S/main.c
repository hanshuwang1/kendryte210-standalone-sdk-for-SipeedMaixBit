#include "lcd.h"
#include "sysctl.h"
#include "plic.h"
#include "sleep.h"
#include "gpiohs.h"
#include <stdio.h>

void io_set_power(void) {
    sysctl_set_power_mode(SYSCTL_POWER_BANK0, SYSCTL_POWER_V33);
	sysctl_set_power_mode(SYSCTL_POWER_BANK1, SYSCTL_POWER_V33);
	sysctl_set_power_mode(SYSCTL_POWER_BANK2, SYSCTL_POWER_V33);
	sysctl_set_power_mode(SYSCTL_POWER_BANK3, SYSCTL_POWER_V33);
	sysctl_set_power_mode(SYSCTL_POWER_BANK4, SYSCTL_POWER_V33);
	sysctl_set_power_mode(SYSCTL_POWER_BANK5, SYSCTL_POWER_V33);
    sysctl_set_power_mode(SYSCTL_POWER_BANK6, SYSCTL_POWER_V18);
    sysctl_set_power_mode(SYSCTL_POWER_BANK7, SYSCTL_POWER_V18);

    /* 设置系统时钟频率 */
    sysctl_pll_set_freq(SYSCTL_PLL0, 800000000UL);
    sysctl_pll_set_freq(SYSCTL_PLL1, 400000000UL);
    sysctl_pll_set_freq(SYSCTL_PLL2, 45158400UL);

#ifdef BSP_ENABLE_AI
    sysctl_clock_enable(SYSCTL_CLOCK_AI);
#endif
}

void setup(void){
     /* 设置IO口电压 */
    io_set_power();
    /* 系统中断初始化，并使能全局中断 */
    plic_init();
    sysctl_enable_irq();
    lcd_init();
    
    lcd_draw_string(16, 40, "Hello World", RED);
    lcd_draw_string(16, 60, "Nice to meet you!", BLUE);
    lcd_draw_string(16, 80, "I am Kendryte K210", GREEN);
}


int main(void){
    setup();
    uint8_t i = 0;
    while(1)
    {

    }
}
