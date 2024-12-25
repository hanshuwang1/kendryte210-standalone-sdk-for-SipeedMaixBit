#ifndef _BSP_MIC_MIC_H
#define _BSP_MIC_MIC_H

#include "fpioa.h"

/* 硬件IO 口，与原理图对应 */
#define PIN_MIC_SDIN        (20)
#define PIN_MIC_BCK         (18)
#define PIN_MIC_WS          (19)

/* 软件GPIO 口，与程序对应 */
/* GPIO 口的功能，绑定到硬件IO 口 */
#define FUNC_MIC_WS         FUNC_I2S0_WS
#define FUNC_MIC_SDIN       FUNC_I2S0_IN_D1
#define FUNC_MIC_BCK        FUNC_I2S0_SCLK

/* I2S 通道使能掩码 */
#define I2S_CHANNEL_MASK_0  0x03
#define I2S_CHANNEL_MASK_1  0x0C
#define I2S_CHANNEL_MASK_2  0x30
#define I2S_CHANNEL_MASK_3  0xC0

#define REC_I2S_RX_DMA_BUF_SIZE     4096        /* 定义RX DMA 数组大小 */
#define REC_I2S_RX_FIFO_SIZE        10          /* 定义接收FIFO大小 */
#define MIC_GAIN                    5           /* 麦克风增益值，可以根据实际调大录音的音量 */
#define REC_SAMPLERATE              16000       /* 采样率,44.1Khz */


void mic_i2s_init(void);
int mic_i2s_receive_data_dma(uint32_t *buff);

#endif  // _BSP_MIC_MIC_H
