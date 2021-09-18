#ifndef SSD1963_H
#define SSD1963_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "stm32f4xx_hal.h"

#define LCD_Brightness_DEFAULT		105
#define LCD_Contrast_DEFAULT			64
#define LCD_Saturation_DEFAULT		64

#define LCD_Brightness_MIN		15
#define LCD_Brightness_MAX		150
#define LCD_Brightness_STEP		15

//=============================================================================
// Configuration
//=============================================================================
//#define SSD1963_DATAPORT	GPIOH
//#define SSD1963_DATAPORT2	GPIOI
//#define SSD1963_CTRLPORT	GPIOE

//#define SSD1963_PIN_RD	(1 << 4)
//#define SSD1963_PIN_WR	(1 << 5)
//#define SSD1963_PIN_CS	(1 << 7)
//#define SSD1963_PIN_RS	(1 << 13)
//#define SSD1963_PIN_RST	(1 << 15)

//#define TFT320240
#define TFT800480
//#define TFT640480
//#define TFTCUSTOM

//=============================================================================
// All numeric constants with "ULL" suffix !
//=============================================================================

#ifdef TFTCUSTOM
#define TFT_FPS 10ULL
#define TFT_WIDTH				480ULL
#define TFT_HSYNC_BACK_PORCH	2ULL
#define TFT_HSYNC_FRONT_PORCH	2ULL
#define TFT_HSYNC_PULSE			41ULL

#define TFT_HEIGHT				272ULL
#define TFT_VSYNC_BACK_PORCH	2ULL
#define TFT_VSYNC_FRONT_PORCH	2ULL
#define TFT_VSYNC_PULSE			10ULL

#endif

#ifdef TFT320240
	#define TFT_WIDTH				320ULL
	#define TFT_HSYNC_BACK_PORCH	40ULL
	#define TFT_HSYNC_FRONT_PORCH	20ULL
	#define TFT_HSYNC_PULSE			28ULL

	#define TFT_HEIGHT				240ULL
	#define TFT_VSYNC_BACK_PORCH	17ULL
	#define TFT_VSYNC_FRONT_PORCH	4ULL
	#define TFT_VSYNC_PULSE			3ULL
#endif

#ifdef TFT800480
	#define TFT_WIDTH				800ULL
	#define TFT_HSYNC_BACK_PORCH	210LL
	#define TFT_HSYNC_FRONT_PORCH	45ULL
	#define TFT_HSYNC_PULSE			1ULL

	#define TFT_HEIGHT				480ULL
	#define TFT_VSYNC_BACK_PORCH	34ULL
	#define TFT_VSYNC_FRONT_PORCH	10ULL
	#define TFT_VSYNC_PULSE			10ULL
#endif

#ifdef TFT640480

	#define TFT_WIDTH				640ULL
	#define TFT_HSYNC_BACK_PORCH	56ULL
	#define TFT_HSYNC_FRONT_PORCH	8ULL
	#define TFT_HSYNC_PULSE			96ULL

	#define TFT_HEIGHT				480ULL
	#define TFT_VSYNC_BACK_PORCH	41ULL
	#define TFT_VSYNC_FRONT_PORCH	2ULL
	#define TFT_VSYNC_PULSE			2ULL
#endif

typedef struct
{
  __IO uint16_t LCD_REG;
  __IO uint16_t LCD_RAM;
} LCD_TypeDef;

#define CMD_ADDR *(uint16_t *)0x60000000
#define DAT_ADDR *(uint16_t *)0x68000000

#define TFT_LCD_BASE           ((uint32_t)(0x60000000 | 0x08000000))
#define LCD                ((LCD_TypeDef *) TFT_LCD_BASE)

#define	TFT_HSYNC_PERIOD	(TFT_HSYNC_PULSE + TFT_HSYNC_BACK_PORCH + TFT_WIDTH  + TFT_HSYNC_FRONT_PORCH)
#define	TFT_VSYNC_PERIOD	(TFT_VSYNC_PULSE + TFT_VSYNC_BACK_PORCH + TFT_HEIGHT + TFT_VSYNC_FRONT_PORCH)

#define TFT_PCLK	(TFT_HSYNC_PERIOD * TFT_VSYNC_PERIOD * TFT_FPS)
#define LCD_FPR		((TFT_PCLK * 1048576)/100000000)

#define mHIGH(x) (x >> 8)
#define mLOW(x) (x & 0xFF)
//=============================================================================
// SSD1963 commands
//=============================================================================
#define SSD1963_NOP						0x00
#define SSD1963_SOFT_RESET  			0x01
#define SSD1963_GET_POWER_MODE 			0x0A
#define SSD1963_GET_ADDRESS_MODE		0x0B
#define SSD1963_GET_DISPLAY_MODE		0x0D
#define SSD1963_GET_TEAR_EFFECT_STATUS 	0x0E
#define SSD1963_ENTER_SLEEP_MODE		0x10
#define SSD1963_EXIT_SLEEP_MODE			0x11
#define SSD1963_ENTER_PARTIAL_MODE		0x12
#define SSD1963_ENTER_NORMAL_MODE		0x13
#define SSD1963_EXIT_INVERT_MODE		0x20
#define SSD1963_ENTER_INVERT_MODE		0x21
#define SSD1963_SET_GAMMA_CURVE			0x26
#define SSD1963_SET_DISPLAY_OFF			0x28
#define SSD1963_SET_DISPLAY_ON			0x29
#define SSD1963_SET_COLUMN_ADDRESS		0x2A
#define SSD1963_SET_PAGE_ADDRESS		0x2B
#define SSD1963_WRITE_MEMORY_START		0x2C
#define SSD1963_READ_MEMORY_START		0x2E
#define SSD1963_SET_PARTIAL_AREA		0x30
#define SSD1963_SET_SCROLL_AREA			0x33
#define SSD1963_SET_TEAR_OFF			0x34
#define SSD1963_SET_TEAR_ON				0x35
#define SSD1963_SET_ADDRESS_MODE		0x36
#define SSD1963_SET_SCROLL_START		0x37
#define SSD1963_EXIT_IDLE_MODE			0x38
#define SSD1963_ENTER_IDLE_MODE			0x39
#define SSD1963_WRITE_MEMORY_CONTINUE	0x3C
#define SSD1963_READ_MEMORY_CONTINUE	0x3E
#define SSD1963_SET_TEAR_SCANLINE		0x44
#define SSD1963_GET_SCANLINE			0x45
#define SSD1963_READ_DDB				0xA1
#define SSD1963_SET_LCD_MODE			0xB0
#define SSD1963_GET_LCD_MODE			0xB1
#define SSD1963_SET_HORI_PERIOD			0xB4
#define SSD1963_GET_HORI_PERIOD			0xB5
#define SSD1963_SET_VERT_PERIOD			0xB6
#define SSD1963_GET_VERT_PERIOD			0xB7
#define SSD1963_SET_GPIO_CONF			0xB8
#define SSD1963_GET_GPIO_CONF			0xB9
#define SSD1963_SET_GPIO_VALUE			0xBA
#define SSD1963_GET_GPIO_STATUS			0xBB
#define SSD1963_SET_POST_PROC			0xBC
#define SSD1963_GET_POST_PROC			0xBD
#define SSD1963_SET_PWM_CONF			0xBE
#define SSD1963_GET_PWM_CONF			0xBF
#define SSD1963_GET_LCD_GEN0			0xC0
#define SSD1963_SET_LCD_GEN0			0xC1
#define SSD1963_GET_LCD_GEN1			0xC2
#define SSD1963_SET_LCD_GEN1			0xC3
#define SSD1963_GET_LCD_GEN2			0xC4
#define SSD1963_SET_LCD_GEN2			0xC5
#define SSD1963_GET_LCD_GEN3			0xC6
#define SSD1963_SET_LCD_GEN3			0xC7
#define SSD1963_SET_GPIO0_ROP			0xC8
#define SSD1963_GET_GPIO0_ROP			0xC9
#define SSD1963_SET_GPIO1_ROP			0xCA
#define SSD1963_GET_GPIO1_ROP			0xCB
#define SSD1963_SET_GPIO2_ROP			0xCC
#define SSD1963_GET_GPIO2_ROP			0xCD
#define SSD1963_SET_GPIO3_ROP			0xCE
#define SSD1963_GET_GPIO3_ROP			0xCF
#define SSD1963_SET_DBC_CONF			0xD0
#define SSD1963_GET_DBC_CONF			0xD1
#define SSD1963_SET_DBC_TH				0xD4
#define SSD1963_GET_DBC_TH				0xD5
#define SSD1963_SET_PLL					0xE0
#define SSD1963_SET_PLL_MN				0xE2
#define SSD1963_GET_PLL_MN				0xE3
#define SSD1963_GET_PLL_STATUS			0xE4
#define SSD1963_SET_DEEP_SLEEP			0xE5
#define SSD1963_SET_LSHIFT_FREQ			0xE6
#define SSD1963_GET_LSHIFT_FREQ			0xE7
#define SSD1963_SET_PIXEL_DATA_INTERFACE 0xF0
#define SSD1963_PDI_8BIT			0
#define SSD1963_PDI_12BIT			1
#define SSD1963_PDI_16BIT			2
#define SSD1963_PDI_16BIT565		3
#define SSD1963_PDI_18BIT			4
#define SSD1963_PDI_24BIT			5
#define SSD1963_PDI_9BIT			6
#define SSD1963_GET_PIXEL_DATA_INTERFACE 0xF1

#define LCD_DIR_HORIZONTAL 	0
#define LCD_DIR_VERTICAL		1



void SSD1963_WriteCommand(uint16_t cmd);
void SSD1963_WriteData(uint16_t data);
uint16_t SSD1963_ReadData(void);
uint16_t H24_RGB565(uint8_t reverse, uint32_t color24);
void SSD1963_Tearing(uint8_t tearing_mode);
void SSD1963_backlight(uint8_t level);
void SSD1963_FillLineFrameBuffer(uint16_t* pData, uint32_t width);
void SSD1963_FillBlockFrameBuffer(uint16_t* pData, uint32_t width, uint32_t height);
void SSD1963_Init (void);
void LCD_Display_Off(void);
void LCD_Display_On(void);
void LCD_Clear(uint16_t Color);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction);
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t Color);
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t Color);
void Lcd_BackLight_Control(uint8_t Stat);
void LCD_Set_Post_Proc(uint8_t Contrast, uint8_t Brightness, uint8_t Saturation);
void LCD_Get_Post_Proc(void);
void LCD_Rotattion(uint8_t val);
void LCD_Delay(__IO uint32_t nCount);
void LCD_WriteRAM_RGB(uint16_t RGB_Code);
void LCD_WriteRAM_Prepare(void);
uint16_t RGB_TO_565(int r , int g , int b);
void Test_Func(void);


// TOUCHGFX - implemented in TOUCHGFXHALL.cpp
void touchgfx_signalVSync(void);
//void touchgfx_signalTransmitComplete(void);


#define LCD_RD_PIN GPIO_PIN_4
#define LCD_RD_PORT GPIOD
#define LCD_WR_PIN GPIO_PIN_5
#define LCD_WR_PORT GPIOD
#define LCD_RS_PIN GPIO_PIN_0
#define LCD_RS_PORT GPIOF
#define LCD_CS_PIN GPIO_PIN_10
#define LCD_CS_PORT GPIOG
//#define LCD_RST_PIN  GPIO_PIN_0
//#define LCD_RST_PORT GPIOG



#define LCD_RD_S HAL_GPIO_WritePin(LCD_RD_PORT, LCD_RD_PIN, GPIO_PIN_SET);
#define LCD_RD_R HAL_GPIO_WritePin(LCD_RD_PORT, LCD_RD_PIN, GPIO_PIN_RESET);
#define LCD_WR_S HAL_GPIO_WritePin(LCD_WR_PORT, LCD_WR_PIN, GPIO_PIN_SET);
#define LCD_WR_R HAL_GPIO_WritePin(LCD_WR_PORT, LCD_WR_PIN, GPIO_PIN_RESET);
#define LCD_RS_S HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET);
#define LCD_RS_R HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET);
#define LCD_CS_S HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
#define LCD_CS_R HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
//#define LCD_RST_S HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
//#define LCD_RST_R HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);

void SSD1963_GPIO_ON(void);
void SSD1963_GPIO_OFF(void);
void SSD1963_GPIO_Toggle(void);
void LCD_DrawPixels(uint16_t *p, uint32_t num, int16_t Xpos, int16_t Ypos);
void LCD_drawBuffer(uint16_t *p, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif/* SSD1963_H */
