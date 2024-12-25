#include "fpioa.h"
#include "gpio.h"
#include "sleep.h"
//硬件IO口，与原理图对应
#define PIN_LED_B           (14)
#define PIN_LED_R           (13)
#define PIN_LED_G           (12)
//软件GPIO口，与程序对应
#define LED_BLUE_GPIONUM    (0)
#define LED_RED_GPIONUM     (1)
#define LED_GREEN_GPIONUM   (2)
//GPIO口的功能，绑定到硬件IO口
#define FUNC_LED_BLUE       (FUNC_GPIO0 + LED_BLUE_GPIONUM)     //其实这用到了嵌入式常见编程方式--偏移
#define FUNC_LED_RED        (FUNC_GPIO0 + LED_RED_GPIONUM)      //FUNC_GPIO0 + LED_RED_GPIONUM = FUNC_GPIO1
#define FUNC_LED_GREEN      (FUNC_GPIO0 + LED_GREEN_GPIONUM)    //FUNC_GPIO0 + LED_GREEN_GPIONUM = FUNC_GPIO2


int main(void)
{
    /* (1)fpio绑定功能到IO引脚 */
    fpioa_set_function(PIN_LED_B, FUNC_LED_BLUE); 
    fpioa_set_function(PIN_LED_R, FUNC_LED_RED); 
    fpioa_set_function(PIN_LED_G, FUNC_LED_GREEN);
    /* (2)初始化GPIO 使能GPIO时钟 */
    gpio_init();
    /* (3)设置GPIO引脚为输出模式 */
    gpio_set_drive_mode(LED_BLUE_GPIONUM, GPIO_DM_OUTPUT);
    gpio_set_drive_mode(LED_RED_GPIONUM, GPIO_DM_OUTPUT);
    gpio_set_drive_mode(LED_GREEN_GPIONUM, GPIO_DM_OUTPUT);
    while(1)
    {
        gpio_set_pin(LED_RED_GPIONUM, GPIO_PV_LOW);
        gpio_set_pin(LED_GREEN_GPIONUM, GPIO_PV_LOW);
        gpio_set_pin(LED_BLUE_GPIONUM, GPIO_PV_HIGH);
        msleep(500);
        gpio_set_pin(LED_BLUE_GPIONUM, GPIO_PV_LOW);
        gpio_set_pin(LED_GREEN_GPIONUM, GPIO_PV_LOW);
        gpio_set_pin(LED_RED_GPIONUM, GPIO_PV_HIGH);
        msleep(500);
        gpio_set_pin(LED_BLUE_GPIONUM, GPIO_PV_LOW);
        gpio_set_pin(LED_RED_GPIONUM, GPIO_PV_LOW);
        gpio_set_pin(LED_GREEN_GPIONUM, GPIO_PV_HIGH);
        msleep(500);
    }
}
