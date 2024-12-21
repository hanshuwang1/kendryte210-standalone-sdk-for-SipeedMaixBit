#ifndef __LCD_PIN_CONFIG_H__
#define __LCD_PIN_CONFIG_H__

#include "fpioa.h"

// LCD硬件IO口，与原理图对应
#define PIN_LCD_CS             (36)     //片选
#define PIN_LCD_RST            (37)     //复位
#define PIN_LCD_RS             (38)     //命令/数据选择
#define PIN_LCD_WR             (39)     //写入信号

// LCD软件GPIO口，与程序对应
#define LCD_RST_GPIONUM        (0)
#define LCD_RS_GPIONUM         (1)

// LCD GPIO口的功能，绑定到硬件IO口
#define FUNC_LCD_CS             (FUNC_SPI0_SS3)                   //片选
#define FUNC_LCD_RST            (FUNC_GPIOHS0 + LCD_RST_GPIONUM)  //GPIOHS0 + LCD_RST_GPIONUM = GPIOHS0 复位
#define FUNC_LCD_RS             (FUNC_GPIOHS0 + LCD_RS_GPIONUM)   //GPIOHS0 + LCD_RS_GPIONUM = GPIOHS1 命令/数据选择
#define FUNC_LCD_WR             (FUNC_SPI0_SCLK)                  //时钟

#endif // __LCD_PIN_CONFIG_H__
