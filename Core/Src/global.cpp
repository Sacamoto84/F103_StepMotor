#include "global.h"


SimpleCLI cli;
BLE ble;
classLog Log;

TFT tft;

//SSD1306 128 64 1 bit
uint8_t LCD_Buffer[128 * 8 + 16];

TFT_LCD_t LCD_0 = { 128,                  // Ширина экрана
		64,                   // Высота экрана
		SSD1306,              // Драйвер
		1,                    // bit
		NULL,                 // notUse0
		&hi2c2,	              // I2C
		0x78,                 // адресс
		NULL,                 // 16 бит буффер
		&LCD_Buffer[1],       // !16 бит буффер
		};

GStepper2<STEPPER2WIRE> stepper(200, STEP, DIR, EN); // драйвер step-dir + пин enable

uint16_t     settingMiliAmper    = 800;    //V0
uint16_t     settingSteps        = 200;    //V1
uint16_t     settingMicrostep    = 1;      //V2
uint16_t     settingMotorOnOff   = 1;      //V3
uint16_t     settingMaxSpeed     = 100;    //V4
uint16_t     settingAcceleration = 2000;   //V5
uint16_t     settingTarget       = 0;      //V6
uint16_t     settingReady        = 0;      //V7
