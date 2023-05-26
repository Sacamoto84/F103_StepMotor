#include "main.h"
#include "TFT.h"
#include "i2c.h"
#include "FontClassic/FontClassic.h"

#include "logUART.h"

#include "GyverStepper2.h"
#include "ArduinoPin.h"

#include "SimpleCLI.h"
#include "BLE.h"

SimpleCLI cli;

BLE ble;

classLog Log;

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

TFT tft;

#define STEP 0
#define DIR  1
#define EN   2
#define M1   3
#define M2   4
#define M3   5

GStepper2<STEPPER2WIRE> stepper(200, STEP, DIR, EN);// драйвер step-dir + пин enable

constexpr int a[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
constexpr int length_a = sizeof(a) / sizeof(int);

extern "C" void BLE_IRQHandle(void)
{
  ble.USART_RX_IRQHandler();
}


void cliCallBack(char * data)
{
  cli.parse(data);
}

extern "C" void setup(void) {

	USART2->BRR = 0x12; //2MHz
	Log.init(&huart2);

	Log.reset();
	Log.clear();
	Log.i ((char*)"Запуск");
	Log.e((char*)"Ошибка");

	ble.init(&huart1);

    ble.setCallback(cliCallBack);

	//USART1->CR1 |= USART_CR1_RXNEIE;


	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

	Log.i("1");

	int z = length_a;
	z++;

	setArduinoPin(STEP, STEP_GPIO_Port, STEP_Pin);
	setArduinoPin(DIR, DIR_GPIO_Port, DIR_Pin);
	setArduinoPin(EN, EN_GPIO_Port, EN_Pin);

	setArduinoPin(M1, M1_GPIO_Port, M1_Pin);
	setArduinoPin(M2, M2_GPIO_Port, M2_Pin);
	setArduinoPin(M3, M3_GPIO_Port, M3_Pin);

	digitalWrite(M1, 1);
	digitalWrite(M2, 1);
	digitalWrite(M3, 1);

	tft.init(&LCD_0);
	tft.Fill(0);
	tft.driver.SSD1306_Contrast(200);
	tft.SetFontColor(1, 0);

	FontClassicPuts(&tft, (char*) "HeloПр123", &Font_7x10, 0);
	tft.driver.Update();

	stepper.invertEn(true);

	stepper.enable();
	//stepper.disable();

	stepper.setMaxSpeed(100.0F);     // скорость движения к цели
	stepper.setAcceleration(2000); // ускорение
	stepper.setTarget(3000000);       // цель

	while (1) {
		//stepper.tick(); //Перенос в таймер

        ble.Task();

	}
}

extern "C" void update(void) {
	tft.driver.Update();
}





