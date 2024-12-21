#include "st7789.h"
#include "gpiohs.h"
#include "spi.h"
#include "lcd_pin_config.h"
#include "sysctl.h"

 /* 初始化RS命令数据选择引脚GPIO为输出模式 */
static void init_rs_gpio(void) {
    gpiohs_set_drive_mode(LCD_RS_GPIONUM, GPIO_DM_OUTPUT);
    gpiohs_set_pin(LCD_RS_GPIONUM, GPIO_PV_HIGH);   // 默认为数据传输
}

/* 初始化LCD复位引脚GPIO */
static void init_rst(void) {
    gpiohs_set_drive_mode(LCD_RST_GPIONUM, GPIO_DM_OUTPUT);
    gpiohs_set_pin(LCD_RST_GPIONUM, GPIO_PV_HIGH);
}

/* 开始传输命令 */
static void set_start_cmd(void) {
    gpiohs_set_pin(LCD_RS_GPIONUM, GPIO_PV_LOW);
}

/* 开始传输数据 */
static void set_start_data(void) {
    gpiohs_set_pin(LCD_RS_GPIONUM, GPIO_PV_HIGH);
}

/* 设置LCD-RST电平 */
static void set_rst(uint8_t val) {
    gpiohs_set_pin(LCD_RST_GPIONUM, val);
}

/* ST7789底层初始化 */
void tft_hard_init(void) {
    /* (1)绑定引脚功能 */
    fpioa_set_function(PIN_LCD_CS, FUNC_LCD_CS);
    fpioa_set_function(PIN_LCD_RST, FUNC_LCD_RST);
    fpioa_set_function(PIN_LCD_RS, FUNC_LCD_RS);
    fpioa_set_function(PIN_LCD_WR, FUNC_LCD_WR);
    /* (2)设置SPI0_D0-D7 DVP_D0-D7为SPI和DVP数据引脚 */
    sysctl_set_spi0_dvp_data(1);
    /* (3)设置BANK6电源域的电压为1.8V 根据LCD手册 */
    sysctl_set_power_mode(SYSCTL_POWER_BANK6, SYSCTL_POWER_V18);
    /* (4)初始化GPIOHS */
    init_rs_gpio();
    init_rst();
    set_rst(0);
    /* (5)初始化SPI */
    spi_init(LCD_SPI, SPI_WORK_MODE_0, SPI_FF_OCTAL, 8, 0);
    spi_set_clk_rate(LCD_SPI, LCD_SPI_CLK_RATE);
    set_rst(1);
}

/**
  * @brief spi写命令
  * @param cmd: 命令
  * @retval None
  */
void tft_write_command(uint8_t cmd) {
    set_start_cmd();
    spi_init(LCD_SPI, SPI_WORK_MODE_0, SPI_FF_OCTAL, 8, 0);
    spi_init_non_standard(LCD_SPI, 8 /*instrction length*/, 0 /*address length*/, 0 /*wait cycles*/,
        SPI_AITM_AS_FRAME_FORMAT /*spi address trans mode*/);
    spi_send_data_normal_dma(LCD_SPI_DMA_CH, LCD_SPI, LCD_SPI_CS_NUM, (uint8_t*)(&cmd), 1, SPI_TRANS_CHAR);
}

/**
  * @brief spi写一个字节数据(uint8_t类型)
  * @param data_buf: 数据缓存区地址指针
  * @param length: 数据长度
  * @retval None
  */
void tft_write_byte(uint8_t* data_buf, uint32_t length) {
    set_start_data();
    spi_init(LCD_SPI, SPI_WORK_MODE_0, SPI_FF_OCTAL, 8, 0);
    spi_init_non_standard(LCD_SPI, 8 /*instrction length*/, 0 /*address length*/, 0 /*wait cycles*/,
        SPI_AITM_AS_FRAME_FORMAT /*spi address trans mode*/);
    spi_send_data_normal_dma(LCD_SPI_DMA_CH, LCD_SPI, LCD_SPI_CS_NUM, data_buf, length, SPI_TRANS_CHAR);
}

/**
  * @brief spi写半字数据(uint16_t类型)
  * @param data_buf: 数据缓存区地址指针
  * @param length: 数据长度
  * @retval None
  */
void tft_write_half(uint16_t* data_buf, uint32_t length) {
    set_start_data();
    spi_init(LCD_SPI, SPI_WORK_MODE_0, SPI_FF_OCTAL, 16, 0);
    spi_init_non_standard(LCD_SPI, 16 /*instrction length*/, 0 /*address length*/, 0 /*wait cycles*/,
        SPI_AITM_AS_FRAME_FORMAT /*spi address trans mode*/);
    spi_send_data_normal_dma(LCD_SPI_DMA_CH, LCD_SPI, LCD_SPI_CS_NUM, data_buf, length, SPI_TRANS_SHORT);
}

/**
  * @brief spi写一个字数据(uint32_t类型)
  * @param data_buf: 数据缓存区地址指针
  * @param length: 数据长度
  * @retval None
  */
void tft_write_word(uint32_t* data_buf, uint32_t length, uint32_t flag) {
    set_start_data();
    spi_init(LCD_SPI, SPI_WORK_MODE_0, SPI_FF_OCTAL, 32, 0);
    spi_init_non_standard(LCD_SPI, 0 /*instrction length*/, 32 /*address length*/, 0 /*wait cycles*/,
        SPI_AITM_AS_FRAME_FORMAT /*spi address trans mode*/);
    spi_send_data_normal_dma(LCD_SPI_DMA_CH, LCD_SPI, LCD_SPI_CS_NUM, data_buf, length, SPI_TRANS_INT);
}

/**
  * @brief spi填充相同数据(uint32_t类型)
  * @param data_buf: 数据缓存区地址指针
  * @param length: 数据长度
  * @retval None
  */
void tft_fill_data(uint32_t* data_buf, uint32_t length) {
    set_start_data();
    spi_init(LCD_SPI, SPI_WORK_MODE_0, SPI_FF_OCTAL, 32, 0);
    spi_init_non_standard(LCD_SPI, 0 /*instrction length*/, 32 /*address length*/, 0 /*wait cycles*/,
        SPI_AITM_AS_FRAME_FORMAT /*spi address trans mode*/);
    spi_fill_data_dma(LCD_SPI_DMA_CH, LCD_SPI, LCD_SPI_CS_NUM, data_buf, length);

}

#if 0
void tft_fill_data16(uint16_t* data_buf, uint32_t length) {
    set_start_data();
    spi_init(LCD_SPI, SPI_WORK_MODE_0, SPI_FF_OCTAL, 16, 0);
    spi_init_non_standard(LCD_SPI, 16 /*instrction length*/, 0 /*address length*/, 0 /*wait cycles*/,
        SPI_AITM_AS_FRAME_FORMAT /*spi address trans mode*/);
    spi_fill_data_dma(LCD_SPI_DMA_CH, LCD_SPI, LCD_SPI_CS_NUM, data_buf, length);
}
#endif