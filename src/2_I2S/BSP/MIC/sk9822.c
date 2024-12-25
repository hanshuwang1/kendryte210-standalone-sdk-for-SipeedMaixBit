#include <stdint.h>
#include "fpioa.h"
#include "gpio.h"
#include "sleep.h"
#include "sk9822.h"


static frame_t sk9822_buf[SK9822_NUM];
/**
  * @brief SK9822初始化
  * @param None
  * @retval None
  */
void sk9822_init(void)
{
    /* (1)fpio绑定功能到IO引脚 */
    fpioa_set_function(PIN_SK9822_CLK, FUNC_SK9822_CLK);
    fpioa_set_function(PIN_SK9822_DAT, FUNC_SK9822_DAT);
    /* (2)初始化GPIO 使能GPIO时钟 */
    gpio_init();
    /* (3)设置GPIO引脚为输出模式 */
    gpio_set_drive_mode(SK9822_CLK_GPIONUM, GPIO_DM_OUTPUT);
    gpio_set_drive_mode(SK9822_DAT_GPIONUM, GPIO_DM_OUTPUT);
    /* (4)设置GPIO引脚默认高电平电平 */
    gpio_set_pin(SK9822_CLK_GPIONUM, GPIO_PV_HIGH);
}

/**
  * @brief SK9822发送数据
  * @param data: 要发送的数据
  * @retval None
  */
void _sk9822_send_data(uint8_t data)
{
    uint8_t i = 0;
	
	SK9822_CLK(1);
	usleep(1);
	
	for (i = 0; i < 8; i++)
	{
		SK9822_CLK(0);
		usleep(1);
		
		SK9822_DAT(data>>(7-i) & 0x01);
		usleep(1);
		/* 上升沿 */
		SK9822_CLK(1);
		usleep(1);
    }
}

/**
  * @brief 帧头
  * @param None
  * @retval None
  */
void _start_frame(void)
{
    uint8_t i = 0;
    for (i = 0; i < 4; i++)
    {
        _sk9822_send_data(0x00);
    }
}

/**
  * @brief 帧尾
  * @param None
  * @retval None
  */
void _end_frame(void)
{
    uint8_t i = 0;
    for (i = 0; i < 4; i++)
    {
        _sk9822_send_data(0xFF);
    }
}

/**
  * @brief 设置整体颜色亮度
  * @param None
  * @retval None
  */
void _set_brightness_color(void)
{
    uint8_t i = 0;
    _start_frame();
    for (i = 0; i < SK9822_NUM; i++)
    {
        _sk9822_send_data(sk9822_buf[i].brightness | 0xE0);     /* 亮度LSB五位控制 */
        _sk9822_send_data(sk9822_buf[i].blue);
        _sk9822_send_data(sk9822_buf[i].green);
        _sk9822_send_data(sk9822_buf[i].red);
    }
    _end_frame();
}

/**
  * @brief 设置单个颜色亮度
  * @param n_led: 第几个灯
  * @param color: 颜色
  * @retval None
  */
void sk9822_set_one_color(uint8_t n_led, frame_t frame)
{
    if(n_led > SK9822_NUM) return;
    frame.brightness |= 0xE0;
    sk9822_buf[n_led] = frame;
    _set_brightness_color();
}

/**
  * @brief 设置整体颜色亮度
  * @param color: 颜色
  * @retval None
  */
void sk9822_set_all_color(frame_t frame)
{
    uint8_t i = 0;
    _start_frame();
    for (i = 0; i < SK9822_NUM; i++)
    {
        sk9822_buf[i] = frame;
        _sk9822_send_data(sk9822_buf[i].brightness | 0xE0);
        _sk9822_send_data(sk9822_buf[i].blue);
        _sk9822_send_data(sk9822_buf[i].green);
        _sk9822_send_data(sk9822_buf[i].red);
    }
    _end_frame();
}
