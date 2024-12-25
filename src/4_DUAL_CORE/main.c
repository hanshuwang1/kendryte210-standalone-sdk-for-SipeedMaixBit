#include "bsp.h"
#include <stdint.h>
#include <stdio.h>
#include "sysctl.h"

int core1_main(void *ctx)
{
    uint8_t state = 1;
    uint16_t core = current_coreid();
    printf("Core %d says hello\n", core);
    while(1)
    {
        msleep(500);
        if(state = !state)
        {
            printf("Core %d is running too\n", core);
        }
        else
        {
            printf("Core %d is running faster\n", core);
        }
    }
}

int main(void)
{
    uint16_t core = current_coreid();
    printf("Core %d says i'm here\n", core);
    register_core1(core1_main, NULL);
    while (1)
    {
        sleep(1);
        printf("Core %d : running\n", core);
    }
}