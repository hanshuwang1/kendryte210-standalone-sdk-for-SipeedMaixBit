#include <stdio.h>
#include <stdint.h>
#include "sysctl.h"
#include "plic.h"
#include "sleep.h"
#include "gpiohs.h"
#include "dmac.h"
#include "iomem.h"
#include "i2s.h"
#include "./BSP/MIC/mic.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/TIMER/interrupt.h"
#include "./BSP/MIC/sk9822.h"

uint32_t rx_buff[1];
uint8_t i=0;

void io_set_power(void) {
    sysctl_set_power_mode(SYSCTL_POWER_BANK0, SYSCTL_POWER_V33);
	sysctl_set_power_mode(SYSCTL_POWER_BANK1, SYSCTL_POWER_V33);
	sysctl_set_power_mode(SYSCTL_POWER_BANK2, SYSCTL_POWER_V33);
	sysctl_set_power_mode(SYSCTL_POWER_BANK3, SYSCTL_POWER_V33);
	sysctl_set_power_mode(SYSCTL_POWER_BANK4, SYSCTL_POWER_V33);
	sysctl_set_power_mode(SYSCTL_POWER_BANK5, SYSCTL_POWER_V33);
    sysctl_set_power_mode(SYSCTL_POWER_BANK6, SYSCTL_POWER_V18);    // 设置BANK6电源域的电压为1.8V LCD
    sysctl_set_power_mode(SYSCTL_POWER_BANK7, SYSCTL_POWER_V18);    // 设置BANK7电源域的电压为1.8V DVP

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

    lcd_init();     /* LCD初始化 */
    dmac_init();    /* DMA初始化 */
    // gtimer_init();  /* 定时器初始化 */
    // mic_i2s_init(); /* MIC I2S初始化 */
    sk9822_init();  /* SK9822初始化 */
    
    lcd_draw_string(16, 0, "I am Kendryte K210", PINK);
}

uint32_t gradient_table[16] = {  
    0xffbe0e,
    0xfab61e,
    0xf5ad2e,
    0xf0a53e,
    0xeb9d4e,
    0xe6955e,
    0xe18c6e,
    0xdc847e,
    0xd67c8f,
    0xd1749f,
    0xcc6baf,
    0xc763bf,
    0xc25bcf,
    0xbd53df,
    0xb84aef,
    0xb342ff};

int main(void){
    setup();
    uint8_t i = 0;
    frame_t my_frame;
    for(i=0; i<16; i++){
        my_frame.brightness = 0xE5;
        my_frame.blue = gradient_table[i]>>16;
        my_frame.green = gradient_table[i]>>8 & 0x0000ff;
        my_frame.red = gradient_table[i] & 0x0000ff;
        sk9822_set_all_color(my_frame);
        msleep(500);
        if(i == 15){
            i = 0;
        }
    }
    while(1)
    {
        

        // mic_i2s_receive_data_dma();
        // lcd_show_decimal_num(16, 100, buff, 4, YELLOW);
        // mic_i2s_receive_data_dma(rx_buff);
        // i2s_receive_data_dma(I2S_DEVICE_0, rx_buff, 1, DMAC_CHANNEL1);
        // lcd_show_decimal_num(16, 100, rx_buff[0], 3, YELLOW);
        // sprintf(buf,"%d",rx_buff[0]);
        // lcd_draw_string(16, 120, (char*)buf, YELLOW);      
    }
}
