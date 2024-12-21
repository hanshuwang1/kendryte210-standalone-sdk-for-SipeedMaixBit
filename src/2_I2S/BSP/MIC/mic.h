#ifndef BSP_MIC_MIC_H_
#define BSP_MIC_MIC_H_

#include "fpioa.h"

/* 硬件IO 口，与原理图对应 */
#define PIN_MIC_SDIN (20)
#define PIN_MIC_BCK (18)
#define PIN_MIC_WS (19)

/* 软件GPIO 口，与程序对应 */

/* GPIO 口的功能，绑定到硬件IO 口 */
#define FUNC_MIC_WS FUNC_I2S0_WS
#define FUNC_MIC_SDIN FUNC_I2S0_IN_D1
#define FUNC_MIC_BCK FUNC_I2S0_SCLK

#endif  // BSP_MIC_MIC_H_
