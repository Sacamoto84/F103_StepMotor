#include "main.h"

#include "global.h"




extern "C" void BLE_IRQHandle(void) {
	ble.USART_RX_IRQHandler();
}



void cliCallBack(char *data) {
	cli.parse(data);
}



//extern uint16_t settingMiliAmper;    //V0
//extern uint16_t settingSteps;        //V1
//extern uint16_t settingMicrostep;    //V2
//extern uint16_t settingMotorOnOff;   //V3
//extern uint16_t settingMaxSpeed;     //V4
//extern uint16_t settingAcceleration; //V5
//extern uint16_t settingTarget;       //V6
//extern uint16_t settingReady;        //V7

void lcdInfo(void)
{
	char str[64];
	tft.Fill(0);
	tft.uTFT.CurrentX = 0;
	tft.uTFT.CurrentY = 0;
	sprintf(str,"%d mA", settingMiliAmper);
	FontClassicPuts(&tft, str, &Font_7x10, 0);

	tft.uTFT.CurrentX = 0;
	tft.uTFT.CurrentY = 9;
	sprintf(str,"%d on off", settingMotorOnOff);
	FontClassicPuts(&tft, str, &Font_7x10, 0);


	tft.driver.Update();

}






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



        if ((NOW - time) > 25)
		{
        	DEBUG1;
        	time = NOW;
        	sendAllV(); //190uS -g  126uS -f
        	DEBUG0;
		}

      lcdInfo();

	}
}

extern "C" void update(void) {
	tft.driver.Update();
}

