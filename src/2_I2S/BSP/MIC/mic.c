#include "mic.h"
#include "gpio.h"
#include "i2s.h"

/**
  * @brief 麦克风引脚初始化, 绑定GPIO功能
  * @param None
  * @retval None
  */
void mic_i2s_hardware_init(void)
{
    /* I2S 初始化 */
    gpio_init(); /* 使能GPIO 的时钟 */
    /* mic */
    fpioa_set_function(PIN_MIC_WS, FUNC_MIC_WS);
    fpioa_set_function(PIN_MIC_SDIN, FUNC_MIC_SDIN);
    fpioa_set_function(PIN_MIC_BCK, FUNC_MIC_BCK);
}

/**
  * @brief I2S 初始化
  * @param None
  * @retval None
  */
void init_i2s(void)
{
	printk("init i2s.\n");

	/* I2s init */
    i2s_init(I2S_DEVICE_0, I2S_RECEIVER, 0x3);

    i2s_rx_channel_config(I2S_DEVICE_0, I2S_CHANNEL_0,
            RESOLUTION_16_BIT, SCLK_CYCLES_32,
            TRIGGER_LEVEL_4, STANDARD_MODE);
    i2s_rx_channel_config(I2S_DEVICE_0, I2S_CHANNEL_1,
            RESOLUTION_16_BIT, SCLK_CYCLES_32,
            TRIGGER_LEVEL_4, STANDARD_MODE);
    i2s_rx_channel_config(I2S_DEVICE_0, I2S_CHANNEL_2,
            RESOLUTION_16_BIT, SCLK_CYCLES_32,
            TRIGGER_LEVEL_4, STANDARD_MODE);
    i2s_rx_channel_config(I2S_DEVICE_0, I2S_CHANNEL_3,
            RESOLUTION_16_BIT, SCLK_CYCLES_32,
            TRIGGER_LEVEL_4, STANDARD_MODE);

    i2s_set_sample_rate(I2S_DEVICE_0, 44100);

}