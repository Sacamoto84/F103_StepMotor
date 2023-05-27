#include "main.h"
#include "TFT.h"
#include "i2c.h"
#include "FontClassic/FontClassic.h"

#include "logUART.h"

#include "GyverStepper2.h"
#include "ArduinoPin.h"

#include "SimpleCLI.h"
#include "BLE.h"

#include "mString.h"

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

GStepper2<STEPPER2WIRE> stepper(200, STEP, DIR, EN); // драйвер step-dir + пин enable

constexpr int a[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
constexpr int length_a = sizeof(a) / sizeof(int);

extern "C" void BLE_IRQHandle(void) {
	ble.USART_RX_IRQHandler();
}

void pingCallback(cmd *c) {
	Command cmd(c); // Create wrapper object
	Log.e("Pong!");
}

// Callback in case of an error
void errorCallback(cmd_error *e) {
	CommandError cmdError(e); // Create wrapper object

	Log.e("ERROR: ");
	Log.e(cmdError.toString().c_str());

}

void cliCallBack(char *data) {
	cli.parse(data);
}

void cliAddCommand() {
	Command ping;

	ping = cli.addCmd("ping", pingCallback);

    // Set error Callback
    cli.setOnError(errorCallback);
}

uint16_t     settingMiliAmper    = 800;    //V0
uint16_t     settingSteps        = 200;    //V1
uint16_t     settingMicrostep    = 1;      //V2
uint16_t     settingMotorOnOff   = 1;      //V3
uint16_t     settingMaxSpeed     = 100;    //V4
uint16_t     settingAcceleration = 2000;   //V5
uint16_t     settingTarget       = 0;      //V6
uint16_t     settingReady        = 0;      //V7

mString <1024> stringOut;

void sendAllV(void)
{
	stringOut.clear();
	stringOut += "!V/";
	stringOut.add(settingMiliAmper);
	stringOut += "/";
	stringOut.add(settingSteps);
	stringOut += "/";
	stringOut.add(settingMicrostep);
	stringOut += "/";
	stringOut.add(settingMotorOnOff);
	stringOut += "/";
	stringOut.add(settingMaxSpeed);
	stringOut += "/";
	stringOut.add(settingAcceleration);
	stringOut += "/";
	stringOut.add(settingTarget);
	stringOut += "/222";
	stringOut.add(settingReady);
	uint8_t crc;
	crc = ble.CRC8(&stringOut.buf[1], stringOut.length()-1);
	stringOut += ";";
	stringOut.add(crc);
	stringOut += "$\n";

	HAL_UART_Transmit_DMA(&huart1, (uint8_t*) &stringOut.buf[0], stringOut.length());

}


//char str[1024]={0};
//char crc;
//crc = CRC8(&outstr[0], strlen(outstr));
//sprintf(str, "!%s;%d$", outstr, crc);
//Log.s(str);
//HAL_UART_Transmit(_huart, (uint8_t*) &str[0], strlen(str), 1000);




extern "C" void setup(void) {

	USART2->BRR = 0x12; //2MHz
	Log.init(&huart2);

	Log.reset();
	Log.clear();
	Log.i((char*) "Запуск");
	Log.e((char*) "Ошибка");

	ble.init(&huart1);

	ble.setCallback(cliCallBack);

	cliAddCommand();

	ble.Send((char*)"Hello");

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

	uint32_t time = NOW;

	while (1) {
		//stepper.tick(); //Перенос в таймер

		ble.Task();



        if ((NOW - time) > 50)
		{
        	DEBUG1;
        	time = NOW;
        	sendAllV(); //190uS -g  126uS -f
        	DEBUG0;
		}

	}
}

extern "C" void update(void) {
	tft.driver.Update();
}

