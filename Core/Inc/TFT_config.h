#ifndef INC_TFT_CONFIG_H_
#define INC_TFT_CONFIG_H_

//Выбор используемых интерфейсов
//#define TFT_USE_SPI
#define TFT_USE_I2С

//#define LCD_USE_BL //Использование управление яркостью (не реализовано)

//Для ускорения SetPixel GetPixel можно удалить проверки на ненужные цвета
#define TFT_USE_1BIT
//#define TFT_USE_4BIT
//#define TFT_USE_8BIT
//#define TFT_USE_16BIT

//Признак того что у нас не будет смешивания цветовых битностей, например при разнобитных спрайтов
//или нескольких экранов, в SetPixel убираем проверку типа if (LCD->Bit == 1)
#define TFT_USE_ONLY_ONE_BIT_COLOR


//Выбор используемого драйвера
#define TFT_DRIVER_SSD1306
//#define TFT_DRIVER_ILI9225
//#define TFT_DRIVER_ST7789
//#define TFT_DRIVER_ST7735
//#define TFT_DRIVER_ST7735S
//#define TFT_DRIVER_LCD_USB

//#define TFT_UNIT_VIDEO



#endif /* INC_TFT_CONFIG_H_ */
