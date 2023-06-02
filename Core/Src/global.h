/*
 * global.h
 *
 *  Created on: 27 мая 2023 г.
 *      Author: Ivan
 */

#ifndef SRC_GLOBAL_H_
#define SRC_GLOBAL_H_

#include "TFT.h"
#include "i2c.h"
#include "FontClassic/FontClassic.h"

#include "logUART.h"

#include "GyverStepper2.h"
#include "ArduinoPin.h"

#include "SimpleCLI.h"
#include "BLE.h"

#include "mString.h"

#define STEP 0
#define DIR  1
#define EN   2
#define M1   3
#define M2   4
#define M3   5



extern SimpleCLI cli;
extern BLE ble;
extern classLog Log;
extern TFT tft;
extern TFT_LCD_t LCD_0;

extern GStepper2<STEPPER2WIRE> stepper; // драйвер step-dir + пин enable

void cliAddCommand(void);

void V0Callback(cmd *c);
void V1Callback(cmd *c);
void V2Callback(cmd *c);
void V3Callback(cmd *c);
void V4Callback(cmd *c);
void V5Callback(cmd *c);
void V6Callback(cmd *c);
void V7Callback(cmd *c);
void V8Callback(cmd *c);

void sendAllV(void);



extern uint16_t settingMiliAmper;    //V0
extern uint16_t settingSteps;        //V1
extern uint16_t settingMicrostep;    //V2
extern uint16_t settingMotorOnOff;   //V3
extern uint16_t settingMaxSpeed;     //V4
extern uint16_t settingAcceleration; //V5
extern uint16_t settingTarget;       //V6
extern uint16_t settingReady;        //V7





#endif /* SRC_GLOBAL_H_ */
