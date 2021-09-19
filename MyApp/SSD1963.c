
#include "SSD1963.h"
#include <string.h>
typedef unsigned short u16;

typedef unsigned int u32;

uint16_t CurrentTextColor = 0x0000, CurrentBackColor = 0xFFFF;
uint8_t SSD1963_IO = 0;

void SSD1963_WriteCommand(uint16_t cmd)
{
	LCD_RS_R
	LCD_RD_S
	LCD_CS_R
    LCD->LCD_REG = cmd;
	LCD_WR_R
	LCD_WR_S
	LCD_CS_S
}

void SSD1963_WriteData(uint16_t data)
{
	LCD_RS_S
	LCD_RD_S
	LCD_CS_R
    LCD->LCD_RAM = data;
	LCD_WR_R
	LCD_WR_S
	LCD_CS_S
}

uint16_t SSD1963_ReadData(void)
{
    return LCD->LCD_RAM;
}

uint16_t H24_RGB565(uint8_t reverse, uint32_t color24)
{
    uint8_t b = (color24 >> 16) & 0xFF;
    uint8_t g = (color24 >> 8) & 0xFF;
    uint8_t r = color24 & 0xFF;
    if (reverse) return ((b / 8) << 11) | ((g / 4) << 5) | (r / 8);
    else return ((r / 8) << 11) | ((g / 4) << 5) | (b / 8);
}
/**
 * @brief TE signal is sent from the SSD1963 to the host processor at the start of VFP.
 *
 * @param  Tearing effect line mode. Default after reset value 0.
 *      0 The tearing effect output line consists of V-blanking information only.
 *      1 The tearing effect output line consists of both V-blanking and H-blanking information by
 *          set_tear_scanline (0x44).
 */
void SSD1963_Tearing(uint8_t tearing_mode)
{
    SSD1963_WriteCommand(SSD1963_SET_TEAR_ON);
    if(tearing_mode)
    {
        SSD1963_WriteData(0x01);
    }
    else
    {
        SSD1963_WriteData(0x00);
    }
}
/**
 * @brief Backlight control. Set the PWM configuration.
 *      PWMF[7:0] : Set the PWM frequency in system clock. Default after reset value 0.
 *      PWM[7:0] : Set the PWM duty cycle. Default after reset value 0.
 *      NOTE: If DBC enable (0xD0] A0 = 1, these parameter will be ignored
 *      NOTE: PWM always 0 if PWM[7:0] = 00h
 *      C[3] : PWM configuration. Default after reset value 0.
 *          0 PWM controlled by host
 *          1 PWM controlled by DBC
 *      C[0] : PWM enable. Default after reset value 0.
 *          0 PWM disable
 *          1 PWM enable
 *      D[7:0] : DBC manual brightness. Default after reset value 0.
 *          00 Dimmest
 *          FF brightest
 *      E[7:0] : DBC minimum brightness. Default after reset value 0.
 *          00 Dimmest
 *          FF brightest
 *      F[3:0] : Brightness prescaler. Default after reset value 0. This parameter is valid when
 *          Transition Effect enable (0xD0) A5 = 1
 * @param level 0 to 100%
 */
void SSD1963_backlight(uint8_t level)
{
    level = level * 2.55;
    SSD1963_WriteCommand(SSD1963_SET_PWM_CONF);
    SSD1963_WriteData(0x0006);
    SSD1963_WriteData(level);
    SSD1963_WriteData(0x0009);
    SSD1963_WriteData(0x00f0);
    SSD1963_WriteData(0x0000);
    SSD1963_WriteData(0x0000);
}
/**
 * @brief Writes several data on SSD1963 frame bufer
 *
 * @param pData pointer to data of iternal framebufer
 * @param width width block of frame
 */
void SSD1963_FillLineFrameBuffer(uint16_t* pData, uint32_t width)
{
    for (uint32_t w = 0; w < width; w++)
    {
        SSD1963_WriteData(*pData);
    }
}

/**
 * @brief Writes several data on SSD1963 frame bufer
 *
 * @param pData pointer to data of iternal framebufer
 * @param width width block of frame
 * @param height height block of frame
 */
void SSD1963_FillBlockFrameBuffer(uint16_t* pData, uint32_t width, uint32_t height)
{
    for (uint32_t h = 0; h < height; h++)
    {
        SSD1963_FillLineFrameBuffer(&pData[width*h], width);
    }
}

void SSD1963_Init (void)
{
	LCD_Delay(5); /* delay 50 ms */

	SSD1963_WriteCommand(SSD1963_SOFT_RESET);
	LCD_Delay(20);

	SSD1963_WriteCommand(SSD1963_SOFT_RESET);
	LCD_Delay(20);

	SSD1963_WriteCommand(SSD1963_SET_PLL_MN);
	SSD1963_WriteData(29);	// PLLclk = REFclk * 30 (300MHz)
	SSD1963_WriteData(2);	// SYSclk = PLLclk / 3 (100MHz)
	SSD1963_WriteData(4);  // dummy

	LCD_Delay(20);

	SSD1963_WriteCommand(SSD1963_SET_PLL);
	SSD1963_WriteData(0x01);

	LCD_Delay(20);

	SSD1963_WriteCommand(SSD1963_SET_PLL);
	SSD1963_WriteData(0x03);

	LCD_Delay(20);

	SSD1963_WriteCommand(SSD1963_SET_LCD_MODE);
	SSD1963_WriteData(0x20);
	SSD1963_WriteData(0x00);
	SSD1963_WriteData(mHIGH((TFT_WIDTH-1)));
	SSD1963_WriteData(mLOW((TFT_WIDTH-1)));
	SSD1963_WriteData(mHIGH((TFT_HEIGHT-1)));
	SSD1963_WriteData(mLOW((TFT_HEIGHT-1)));
	SSD1963_WriteData(0x00);

	SSD1963_WriteCommand(SSD1963_SET_ADDRESS_MODE);
	SSD1963_WriteData(0x00);

	SSD1963_WriteCommand(SSD1963_SET_PIXEL_DATA_INTERFACE);
	//SSD1963_WriteData(SSD1963_PDI_8BIT); /*** 8-bit ***/
	SSD1963_WriteData(SSD1963_PDI_16BIT565); /*** RGB565 16-bit: In this mode we don't have correct color or size***/


	SSD1963_WriteCommand(SSD1963_SET_LSHIFT_FREQ);
	SSD1963_WriteData(0x01);
	SSD1963_WriteData(0x78);
	SSD1963_WriteData(0xD3);  //9.2 Mhz

	LCD_Delay(1);

	SSD1963_WriteCommand(SSD1963_SET_HORI_PERIOD);
	SSD1963_WriteData(mHIGH(TFT_HSYNC_PERIOD));
	SSD1963_WriteData(mLOW(TFT_HSYNC_PERIOD));
	SSD1963_WriteData(mHIGH((TFT_HSYNC_PULSE + TFT_HSYNC_BACK_PORCH)));
	SSD1963_WriteData(mLOW((TFT_HSYNC_PULSE + TFT_HSYNC_BACK_PORCH)));
	SSD1963_WriteData(TFT_HSYNC_PULSE);
	SSD1963_WriteData(0x00);
	SSD1963_WriteData(0x00);
	SSD1963_WriteData(0x00);

	SSD1963_WriteCommand(SSD1963_SET_VERT_PERIOD);
	SSD1963_WriteData(mHIGH(TFT_VSYNC_PERIOD));
	SSD1963_WriteData(mLOW(TFT_VSYNC_PERIOD));
	SSD1963_WriteData(mHIGH((TFT_VSYNC_PULSE + TFT_VSYNC_BACK_PORCH)));
	SSD1963_WriteData(mLOW((TFT_VSYNC_PULSE + TFT_VSYNC_BACK_PORCH)));
	SSD1963_WriteData(TFT_VSYNC_PULSE);
	SSD1963_WriteData(0x00);
	SSD1963_WriteData(0x00);

	LCD_Delay(1);

	SSD1963_WriteCommand(SSD1963_SET_COLUMN_ADDRESS); //SET column address
	SSD1963_WriteData(0x00); //SET start column address=0
	SSD1963_WriteData(0x00);
	SSD1963_WriteData(mHIGH((TFT_HEIGHT-1))); //SET end column address=479
	SSD1963_WriteData(mLOW((TFT_HEIGHT-1)));

	LCD_Delay(1);

	SSD1963_WriteCommand(SSD1963_SET_PAGE_ADDRESS); //SET page address
	SSD1963_WriteData(0x00); //SET start page address=0
	SSD1963_WriteData(0x00);
	SSD1963_WriteData(mHIGH((TFT_WIDTH-1))); //SET end page address=271
	SSD1963_WriteData(mLOW((TFT_WIDTH-1)));

	LCD_Delay(1);

	SSD1963_WriteCommand(SSD1963_SET_PWM_CONF);
	SSD1963_WriteData(0x0A);
	SSD1963_WriteData(0xA5);
	SSD1963_WriteData(0x01);
	SSD1963_WriteData(0x00);
	SSD1963_WriteData(0x00);
	SSD1963_WriteData(0x00);

	LCD_Delay(1);

	SSD1963_WriteCommand(SSD1963_SET_GPIO_CONF);
	SSD1963_WriteData(0x03);
	SSD1963_WriteData(0x01);

//	SSD1963_WriteCommand(SSD1963_SET_GPIO_VALUE);
//	SSD1963_WriteData(0x03);

	SSD1963_WriteCommand(SSD1963_ENTER_NORMAL_MODE);
	SSD1963_WriteCommand(SSD1963_EXIT_IDLE_MODE);
	SSD1963_WriteCommand(SSD1963_SET_DISPLAY_ON);		//SET display on

	LCD_Delay(5);

	LCD_Set_Post_Proc(64, 105, 64);

	LCD_Delay(5);
	//LCD_Clear(0x0000);
}
//=============================================================================
// Disable the Display
//=============================================================================
void LCD_Display_Off(void)
{
  /* Display Off */
	SSD1963_WriteCommand(SSD1963_SET_GPIO_VALUE);
	SSD1963_WriteData(0x00);
}

//=============================================================================
// Enable the Display
//=============================================================================
void LCD_Display_On(void)
{
  /* Display On */
	SSD1963_WriteCommand(SSD1963_SET_GPIO_VALUE);
	SSD1963_WriteData(0x03);
}
//=============================================================================
// Fills whole screen specified color
//=============================================================================
void LCD_Clear(uint16_t Color)
{
	int i , j;
	SSD1963_WriteCommand(0x13);
	LCD_SetCursor(0,0);
	SSD1963_WriteCommand(SSD1963_WRITE_MEMORY_START);

	for(i=0 ; i <= TFT_HEIGHT ; i++ )
		for(j=0 ; j < TFT_WIDTH; j++)
		   LCD_WriteRAM_RGB(Color);
}
//=============================================================================
// Set the cursor position
// origin point is Top Left
//=============================================================================
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	if( Xpos > TFT_WIDTH - 1 || Ypos > TFT_HEIGHT - 1 )
		return;

	SSD1963_WriteCommand(SSD1963_SET_COLUMN_ADDRESS);
	SSD1963_WriteData(Xpos / 256);
	SSD1963_WriteData(Xpos % 256);
	SSD1963_WriteData((TFT_WIDTH - 1) / 256);
	SSD1963_WriteData((TFT_WIDTH - 1) % 256);

	SSD1963_WriteCommand(SSD1963_SET_PAGE_ADDRESS);
	SSD1963_WriteData(Ypos / 256);
	SSD1963_WriteData(Ypos % 256);
	SSD1963_WriteData((TFT_HEIGHT - 1) / 256);
	SSD1963_WriteData((TFT_HEIGHT - 1) % 256);
}

//=============================================================================
// Set the Display Window
// origin point is Top Right and Direction is Button_Top and Left_Right
//=============================================================================
void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
	if(/* Xpos < 0 || Ypos < 0 || */(Xpos + Width) > TFT_WIDTH || (Ypos + Height) > TFT_HEIGHT)
		return;

	SSD1963_WriteCommand(SSD1963_SET_COLUMN_ADDRESS);
	SSD1963_WriteData(Xpos / 256);
    SSD1963_WriteData(Xpos % 256);
    SSD1963_WriteData((Xpos + Width-1)/256);
    SSD1963_WriteData((Xpos + Width-1)%256);

    SSD1963_WriteCommand(SSD1963_SET_PAGE_ADDRESS);
	SSD1963_WriteData(Ypos/256);
    SSD1963_WriteData(Ypos%256);
    SSD1963_WriteData((Ypos + Height-1)/256);
    SSD1963_WriteData((Ypos + Height-1)%256);


	SSD1963_WriteCommand(SSD1963_WRITE_MEMORY_START);
}













//=============================================================================
// Draw Line
// Directions : 0 : Horizontal , 1 : Vertical
//=============================================================================
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
  uint32_t i = 0;

  LCD_SetCursor(Xpos, Ypos);
  if(Direction == LCD_DIR_HORIZONTAL)
  {
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM_RGB(CurrentTextColor);
    }
  }
  else
  {
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
      LCD_WriteRAM_RGB(CurrentTextColor);
      Ypos++;
      LCD_SetCursor(Xpos, Ypos);
    }
  }
}
//=============================================================================
// Draw a Rectangle
//=============================================================================
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t Color)
{
	CurrentTextColor = Color;
  LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
  LCD_DrawLine(Xpos , (Ypos+ Height), Width, LCD_DIR_HORIZONTAL);

  LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
  LCD_DrawLine((Xpos + Width - 1), Ypos, Height, LCD_DIR_VERTICAL);
}
//=============================================================================
// Draw Fill Rectangle
//=============================================================================
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, uint16_t Color)
{
	int i;
	CurrentTextColor = Color;
	for(i = 0; i < Height; i++)
		LCD_DrawLine(Xpos, Ypos + i, Width, 0);
}
//=============================================================================
// Set the Backlight Brightness
//=============================================================================
void Lcd_BackLight_Control(uint8_t Stat)
{
//	if( Stat )
//	{
		// setting PWM for LED backlight
		SSD1963_WriteCommand(SSD1963_SET_PWM_CONF);
		SSD1963_WriteData(0x0E);
		SSD1963_WriteData(Stat);   //addde vared shode be in tabea meghdare noor lcd raa tanzim mikonad 0-255
		SSD1963_WriteData(0x01);
		SSD1963_WriteData(0x00);
		SSD1963_WriteData(0x00);
		SSD1963_WriteData(0x00);
//	}else
//	{
//		// setting PWM for LED backlight
//		SSD1963_WriteCommand(SSD1963_SET_PWM_CONF);
//		SSD1963_WriteData(0x0E);
//		SSD1963_WriteData(0x00);   //addde vared shode be in tabea meghdare noor lcd raa tanzim mikonad 0-255
//		SSD1963_WriteData(0x01);
//		SSD1963_WriteData(0x00);
//		SSD1963_WriteData(0x00);
//		SSD1963_WriteData(0x00);
//	}
}
//=============================================================================
// Set the Backlight Brightness
//=============================================================================
void LCD_Set_Post_Proc(uint8_t Contrast, uint8_t Brightness, uint8_t Saturation)
{
	SSD1963_WriteCommand(SSD1963_SET_POST_PROC);
	SSD1963_WriteData(Contrast);
	SSD1963_WriteData(Brightness);
	SSD1963_WriteData(Saturation);
	SSD1963_WriteData(0x01); /*** Enable the postprocessor. ***/
}
void LCD_Get_Post_Proc(void)
{

	/* Write register */
	SSD1963_WriteCommand(SSD1963_GET_POST_PROC);

	/* Read register content */
//	LCD_Contrast = ((uint8_t) LCD->LCD_RAM);
//	LCD_Brightness = ((uint8_t) LCD->LCD_RAM);
//	LCD_Saturation = ((uint8_t) LCD->LCD_RAM);
}

 ////////////////////////////////////////////////////////////

//=============================================================================
//
//=============================================================================
void LCD_Rotattion(uint8_t val)
{
	SSD1963_WriteCommand(SSD1963_SET_ADDRESS_MODE);
	SSD1963_WriteData(val);
}
//=============================================================================
// Insert a delay time
//=============================================================================
void LCD_Delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0;
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}
//=============================================================================
//
//=============================================================================


//=============================================================================
// Writes to the LCD RAM.
// RGB_Code : the pixel color in RGB mode (5-6-5).
//=============================================================================
void LCD_WriteRAM_RGB(uint16_t RGB_Code)
{
//	unsigned char R,G,B;
//	B = (RGB_Code % 32)*8;
//	RGB_Code = RGB_Code/32;
//	G = (RGB_Code % 64)*4;
//	RGB_Code = RGB_Code/64;
//	R = (RGB_Code % 32)*8;
//    /* Write 8-bit GRAM Reg in RGB Order */
//    SSD1963_WriteData(R);
//    SSD1963_WriteData(G);
//    SSD1963_WriteData(B);

	SSD1963_WriteData(RGB_Code);
}
//=============================================================================
// Prepare to write to the LCD RAM Start From Init Point
//=============================================================================
void LCD_WriteRAM_Prepare(void)
{
  SSD1963_WriteCommand(SSD1963_WRITE_MEMORY_START);
}
/*===============================================*/
/*===============================================*/

















/*===============================================*/
/*===============================================*/
//=============================================================================
// Convert RGB to 565 Code
//=============================================================================
uint16_t RGB_TO_565(int r , int g , int b)
{
	uint16_t result;
	result = r/8;
	result*=64;
	result+=g/4;
	result*=32;
	result+=b/8;
	return result;
}
//=============================================================================
// New Functions for SSD1963 GPIO
//=============================================================================
void SSD1963_GPIO_ON(void)
{
	SSD1963_WriteCommand(SSD1963_SET_GPIO_VALUE);
	SSD1963_WriteData(0x03);
}
void SSD1963_GPIO_OFF(void)
{
	SSD1963_WriteCommand(SSD1963_SET_GPIO_VALUE);
	SSD1963_WriteData(0x00);
}
void SSD1963_GPIO_Toggle(void)
{
	if(SSD1963_IO)
	{
		SSD1963_IO = 0;
		SSD1963_GPIO_OFF();
	}
	else
	{
		SSD1963_IO = 1;
		SSD1963_GPIO_ON();
	}
}

//=============================================================================
// New Functions according RA8875 example
//=============================================================================
void LCD_DrawPixels(uint16_t *p, uint32_t num, int16_t Xpos, int16_t Ypos)
{

	// writeReg(RA8875_CURH0, x);
	// writeReg(RA8875_CURH1, x >> 8);
	// writeReg(RA8875_CURV0, y);
	// writeReg(RA8875_CURV1, y >> 8);
	LCD_SetCursor(Xpos, Ypos);

	// writeCMD(RA8875_MRWC);
	LCD_WriteRAM_Prepare();

	while(num--)
	{
		// writeData(*p++);
		SSD1963_WriteData(*p++);
	}
}
extern uint8_t isTransmittingData;
void LCD_drawBuffer(uint16_t *p, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
	int i;
	// LCD_BTE(0, 0, x, y, w, h, 0, 0);
	LCD_SetDisplayWindow(Xpos, Ypos, Width, Height);
//	SSD1963_WriteCommand(SSD1963_SET_ADDRESS_MODE);
//	SSD1963_WriteData(0x84);
	for(i = 0; i < Height; i++)
	{
		LCD_DrawPixels(p + Width * i, Width, Xpos, Ypos + i);
	}

	isTransmittingData = 0;
//	SSD1963_WriteCommand(SSD1963_SET_ADDRESS_MODE);
//	SSD1963_WriteData(0x00);
//	LCD_SetDisplayWindow(0, 0, 800, 480);
}


