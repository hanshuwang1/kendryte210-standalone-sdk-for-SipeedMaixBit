#include "mic.h"
#include "gpio.h"
#include "i2s.h"
#include "dmac.h"

/**
  * @brief 麦克风I2S DMA初始化
  * @param None
  * @retval None
  */
void mic_i2s_init(void)
{
    /* I2S 初始化 */
    /* 使能GPIO 的时钟 */
    gpio_init(); 
    /* mic io */
    fpioa_set_function(PIN_MIC_WS, FUNC_MIC_WS);
    fpioa_set_function(PIN_MIC_SDIN, FUNC_MIC_SDIN);
    fpioa_set_function(PIN_MIC_BCK, FUNC_MIC_BCK);
	printk("init i2s.\n");
    
    /* (1)I2S设备0初始化为接收模式 通道1的通道掩码是0x03 利用通道掩码来选择使能哪个通道*/
    i2s_init(I2S_DEVICE_0, I2S_RECEIVER, I2S_CHANNEL_MASK_1);  
    /* (2)I2S设备0通道1参数设置 接收数据为16位 单个数据32个时钟 DMA触发FIFO深度参数4级 飞利浦格式*/
    i2s_rx_channel_config(I2S_DEVICE_0, I2S_CHANNEL_0,
            RESOLUTION_16_BIT, SCLK_CYCLES_32,
            TRIGGER_LEVEL_4, STANDARD_MODE);
    /* (3)设置I2S采样率 44.1KHz */
    i2s_set_sample_rate(I2S_DEVICE_0, 44100);
    // /* (4)DMA中断回调 */
    // dmac_set_irq(DMAC_CHANNEL1, mic_i2s_receive_data_dma, NULL/* */, 4/*中断优先级*/);
}

/**
  * @brief I2S DMA接收数据
  * @param None
  * @retval None
  */
int mic_i2s_receive_data_dma(uint32_t *buff)
{
    i2s_receive_data_dma(I2S_DEVICE_0, buff, 1, DMAC_CHANNEL1);
    return 0;
}
