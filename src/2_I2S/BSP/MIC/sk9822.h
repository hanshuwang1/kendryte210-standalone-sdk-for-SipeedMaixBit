#ifndef _BSP_MIC_SK9822_H
#define _BSP_MIC_SK9822_H

#include "fpioa.h"

/* 硬件IO 口，与原理图对应 */
#define PIN_SK9822_CLK      (34)
#define PIN_SK9822_DAT      (35)

/* 软件GPIO 口，与程序对应 */
#define SK9822_CLK_GPIONUM  (0)
#define SK9822_DAT_GPIONUM  (1)
/* GPIO 口的功能，绑定到硬件IO 口 */
#define FUNC_SK9822_CLK     (FUNC_GPIO0 + SK9822_CLK_GPIONUM)
#define FUNC_SK9822_DAT     (FUNC_GPIO0 + SK9822_DAT_GPIONUM)

#define SK9822_CLK(x)   do{x? \
                            gpio_set_pin(SK9822_CLK_GPIONUM, GPIO_PV_HIGH):\
                            gpio_set_pin(SK9822_CLK_GPIONUM, GPIO_PV_LOW);\
                        }while(0)

#define SK9822_DAT(x)   do{x? \
                            gpio_set_pin(SK9822_DAT_GPIONUM, GPIO_PV_HIGH):\
                            gpio_set_pin(SK9822_DAT_GPIONUM, GPIO_PV_LOW);\
                        }while(0)

#define SK9822_NUM 12
typedef struct{
    uint8_t brightness;
    uint8_t blue;
    uint8_t green;
    uint8_t red; 
}frame_t;  /* brightness blue green red*/


void sk9822_init(void);
void sk9822_set_one_color(uint8_t n_led, frame_t frame);
void sk9822_set_all_color(frame_t frame);

#endif // _BSP_MIC_SK9822_H