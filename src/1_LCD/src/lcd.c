/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string.h>
#include <unistd.h>
#include "lcd.h"
#include "st7789.h"
#include "font.h"
#include "stdlib.h"
#include <stdio.h>
#include <math.h>

static lcd_ctl_t lcd_ctl;

/* 初始化LCD，设置显示方向和启动显示 */
void lcd_init(void) {
    uint8_t data = 0;

    /* 硬件初始化 */
    tft_hard_init();

    /* 重置LCD */
    tft_write_command(SOFTWARE_RESET);
    usleep(10000);

    /* 关闭睡眠模式 */
    tft_write_command(SLEEP_OFF);
    usleep(10000);

    /* 设置像素格式：65K, 16bit/pixel */
    tft_write_command(PIXEL_FORMAT_SET);
    data = 0x55;  /* 0101 0101*/
    tft_write_byte(&data, 1);
    usleep(10000);

    /* 打开显示反转 */
    // tft_write_command(INVERSION_DISPLAY_ON);
    // usleep(10000);

    /* 设置LCD显示方向 */
    lcd_set_direction(DIR_YX_RLDU);

    /* 反转显示 */
    tft_write_command(INVERSION_DISPLAY_OFF);
    usleep(10000);

    /* 使能显示 */
    tft_write_command(NORMAL_DISPLAY_ON);
    usleep(10000);
    tft_write_command(DISPLAY_ON);

    /* 清空显示 */
    lcd_clear(BLACK);
    printf("lcd inited\r\n");
}

/* 设置LCD显示方向 */
void lcd_set_direction(lcd_dir_t dir) {
    lcd_ctl.dir = dir;
    if (dir & DIR_XY_MASK) {
        lcd_ctl.width = LCD_Y_MAX - 1;
        lcd_ctl.height = LCD_X_MAX - 1;
    }
    else {
        lcd_ctl.width = LCD_X_MAX - 1;
        lcd_ctl.height = LCD_Y_MAX - 1;
    }

    tft_write_command(MEMORY_ACCESS_CTL);
    tft_write_byte((uint8_t*)&dir, 1);
}

/* 设置显示区域 */
void lcd_set_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    uint8_t data[4] = { 0 };

    data[0] = (uint8_t)(x1 >> 8);
    data[1] = (uint8_t)(x1);
    data[2] = (uint8_t)(x2 >> 8);
    data[3] = (uint8_t)(x2);
    tft_write_command(HORIZONTAL_ADDRESS_SET);
    tft_write_byte(data, 4);

    data[0] = (uint8_t)(y1 >> 8);
    data[1] = (uint8_t)(y1);
    data[2] = (uint8_t)(y2 >> 8);
    data[3] = (uint8_t)(y2);
    tft_write_command(VERTICAL_ADDRESS_SET);
    tft_write_byte(data, 4);

    tft_write_command(MEMORY_WRITE);
}

/* 设置显示某个点的颜色 */
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color) {
    // color = SWAP_16(color);
    lcd_set_area(x, y, x, y);
    tft_write_half(&color, 1);
}

/**
  * @brief LCD清楚区域
  * @param x: x坐标
  * @param y: y坐标
  * @param w: 宽度
  * @param h: 高度
  * @retval None
  */
void lcd_clear_area(uint16_t x, uint16_t y, uint8_t w, uint8_t h) {
    uint32_t data = ((uint32_t)BLACK << 16) | (uint32_t)BLACK;
    uint32_t size = w * h / 2;

    lcd_set_area(x, y, x + w, y + h);
    tft_fill_data(&data, size);
}

/**
  * @brief LCD用次方函数
  * @param m: 底数
  * @param n: 指数
  * @retval m^n
  */
static uint32_t lcd_pow(uint8_t m, uint8_t n) {
    uint32_t result = 1;
    while (n--) result *= m;
    return result;
}

/* LCD显示字符 */
void lcd_draw_char(uint16_t x, uint16_t y, char c, uint16_t color) {
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t data = 0;
    lcd_clear_area(x, y, FONT_SIZE/2, FONT_SIZE);
    for (i = 0; i < 16; i++) {
        data = ascii0816[c * 16 + i];
        for (j = 0; j < 8; j++) {
            if (data & 0x80)
                lcd_draw_point(x + j, y, color);
            data <<= 1;
        }
        y++;
    }
}

/**
  * @brief LCD清屏
  * @param None
  * @retval None
  */
void lcd_clear(uint16_t color) {
    uint32_t data = ((uint32_t)color << 16) | (uint32_t)color;
    lcd_set_area(0, 0, lcd_ctl.height, lcd_ctl.width);
    tft_fill_data(&data, LCD_X_MAX * LCD_Y_MAX / 2);
}

/**
  * @brief LCD显示字符串
  * @param X: x坐标
  * @param Y: y坐标
  * @param str: 字符串
  * @param color: 颜色
  * @retval None
  */
void lcd_draw_string(uint16_t x, uint16_t y, char* str, uint16_t color) {
    while (*str) {
        lcd_draw_char(x, y, *str, color);
        str++;
        x += 8;
    }
}

/**
  * @brief LCD显示十进制数字
  * @param x: x坐标
  * @param y: y坐标
  * @param num: 数字
  * @param len: 长度
  * @param color: 颜色
  * @retval None
  */
void lcd_show_decimal_num(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t color) {
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        lcd_draw_char(x + i * FONT_SIZE / 2, y, num / lcd_pow(10, len - i - 1) % 10 + '0', color);
    }
    
}

