#include "global.h"

void pingCallback(cmd *c);
void errorCallback(cmd_error *e);

void VCallback(cmd *c);

void cliAddCommand(void) {

	Command ping = cli.addCmd("ping", pingCallback);
	Command V = cli.addBoundlessCmd("V", VCallback);

	// Set error Callback
	cli.setOnError(errorCallback);
}

void VCallback(cmd *c) {
	Command cmd(c); // Create wrapper object

	Argument argN = cmd.getArg(0);
	Argument argValue = cmd.getArg(1);

	int N = atoi(argN.getValue().c_str());
	int Value = atoi(argValue.getValue().c_str());
	Log.s("V N=%d Value=%d", N, Value);

	switch (N) {

	case 0:
		settingMiliAmper = Value;
		break;

	case 3:
		settingMotorOnOff = Value;
		break;
	}

}

void pingCallback(cmd *c) {
	Command cmd(c); // Create wrapper object
//Log.e("Pong!");
}

// Callback in case of an error
void errorCallback(cmd_error *e) {
	CommandError cmdError(e); // Create wrapper object
	Log.e("ERROR: ");
	Log.e(cmdError.toString().c_str());
}

mString<1024> stringOut;

void sendAllV(void) {
	stringOut.clear();
	stringOut += "!V ";
	stringOut.add(settingMiliAmper);     //V0
	stringOut += " ";
	stringOut.add(settingSteps);         //V1
	stringOut += " ";
	stringOut.add(settingMicrostep);     //V2
	stringOut += " ";
	stringOut.add(settingMotorOnOff);    //V3
	stringOut += " ";
	stringOut.add(settingMaxSpeed);      //V4
	stringOut += " ";
	stringOut.add(settingAcceleration);  //V5
	stringOut += " ";
	stringOut.add(settingTarget);        //V6
	stringOut += " ";
	stringOut.add(settingReady);         //V7
	uint8_t crc;
	crc = ble.CRC8(&stringOut.buf[1], stringOut.length() - 1);
	stringOut += ";";
	stringOut.add(crc);
	stringOut += "$\n";
	HAL_UART_Transmit_DMA(&huart1, (uint8_t*) &stringOut.buf[0],
			stringOut.length());
}
