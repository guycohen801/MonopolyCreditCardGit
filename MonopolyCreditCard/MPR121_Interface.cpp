
#include "MPR121_Interface.h"
#include "mpr121.h"
#include <Wire.h>


boolean touchStates[12]; //to keep track of the previous touch states

//Initialize the MPR121
char MPR121_INIT() {
	/*
	Wire.begin();
	//start MPR121
	if (!MPR121.begin(0x5A)) {
		return MPR121_INIT_FAIL;
	}

	MPR121.setInterruptPin(SetInterrupPin);
	MPR121.setTouchThreshold(SetTouchThreshold);
	MPR121.setReleaseThreshold(SetReleaseThreshold);
	MPR121.updateTouchData();

	return MPR121_INIT_SUCCESS;
	*/

	pinMode(irqpin, INPUT);
	digitalWrite(irqpin, HIGH); //enable pullup resistor
	Wire.begin();
	mpr121_setup();
	return MPR121_INIT_SUCCESS;
}//end MPR121_INIT

//check if an electride was touched and return it's number
/*char Update_Touch_Status() {
	//check if status changed
	if (MPR121.touchStatusChanged()) {
		MPR121.updateTouchData();
		//check which electrode status changed
		for (int electrode = 0; electrode < NumOfElectrodes; electrode++) {
			if (MPR121.isNewTouch(electrode)) {
				return electrode;
			}//end if
			else if (MPR121.isNewRelease(electrode)) {
				return Touch_Status_Not_Changed;
			}
		}//end for
	}//end if
	else {
		return Touch_Status_Not_Changed;
	}

}//end Update_Touch_Status()
*/

char readTouchInputs() {
	if (!checkInterrupt()) {

		//read the touch state from the MPR121
		Wire.requestFrom(0x5A, 2);

		byte LSB = Wire.read();
		byte MSB = Wire.read();

		uint16_t touched = ((MSB << 8) | LSB); //16bits that make up the touch states


		for (char i = 0; i < 12; i++) {  // Check what electrodes were pressed
			if (touched & (1 << i)) {

				if (touchStates[i] == 0) {
					//pin i was just touched
					return i;
				}
				else if (touchStates[i] == 1) {
					//pin i is still being touched
				}

				touchStates[i] = 1;
			}
			else {

				touchStates[i] = 0;

			}//end else

		}//end for

	}//end if
	return Touch_Status_Not_Changed;
}


void mpr121_setup(void) {

	set_register(0x5A, ELE_CFG, 0x00);

	// Section A - Controls filtering when data is > baseline.
	set_register(0x5A, MHD_R, 0x01);
	set_register(0x5A, NHD_R, 0x01);
	set_register(0x5A, NCL_R, 0x00);
	set_register(0x5A, FDL_R, 0x00);

	// Section B - Controls filtering when data is < baseline.
	set_register(0x5A, MHD_F, 0x01);
	set_register(0x5A, NHD_F, 0x01);
	set_register(0x5A, NCL_F, 0xFF);
	set_register(0x5A, FDL_F, 0x02);

	// Section C - Sets touch and release thresholds for each electrode
	set_register(0x5A, ELE0_T, TOU_THRESH);
	set_register(0x5A, ELE0_R, REL_THRESH);

	set_register(0x5A, ELE1_T, TOU_THRESH);
	set_register(0x5A, ELE1_R, REL_THRESH);

	set_register(0x5A, ELE2_T, TOU_THRESH);
	set_register(0x5A, ELE2_R, REL_THRESH);

	set_register(0x5A, ELE3_T, TOU_THRESH);
	set_register(0x5A, ELE3_R, REL_THRESH);

	set_register(0x5A, ELE4_T, TOU_THRESH);
	set_register(0x5A, ELE4_R, REL_THRESH);

	set_register(0x5A, ELE5_T, TOU_THRESH);
	set_register(0x5A, ELE5_R, REL_THRESH);

	set_register(0x5A, ELE6_T, TOU_THRESH);
	set_register(0x5A, ELE6_R, REL_THRESH);

	set_register(0x5A, ELE7_T, TOU_THRESH);
	set_register(0x5A, ELE7_R, REL_THRESH);

	set_register(0x5A, ELE8_T, TOU_THRESH);
	set_register(0x5A, ELE8_R, REL_THRESH);

	set_register(0x5A, ELE9_T, TOU_THRESH);
	set_register(0x5A, ELE9_R, REL_THRESH);

	set_register(0x5A, ELE10_T, TOU_THRESH);
	set_register(0x5A, ELE10_R, REL_THRESH);

	set_register(0x5A, ELE11_T, TOU_THRESH);
	set_register(0x5A, ELE11_R, REL_THRESH);

	// Section D
	// Set the Filter Configuration
	// Set ESI2
	set_register(0x5A, FIL_CFG, 0x04);

	// Section E
	// Electrode Configuration
	// Set ELE_CFG to 0x00 to return to standby mode
	set_register(0x5A, ELE_CFG, 0x0C);  // Enables all 12 Electrodes


										// Section F
										// Enable Auto Config and auto Reconfig
										/*set_register(0x5A, ATO_CFG0, 0x0B);
										set_register(0x5A, ATO_CFGU, 0xC9);  // USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   set_register(0x5A, ATO_CFGL, 0x82);  // LSL = 0.65*USL = 0x82 @3.3V
										set_register(0x5A, ATO_CFGT, 0xB5);*/  // Target = 0.9*USL = 0xB5 @3.3V

	set_register(0x5A, ELE_CFG, 0x0C);
}


boolean checkInterrupt(void) {
	return digitalRead(irqpin);
}


void set_register(int address, unsigned char r, unsigned char v) {
	Wire.beginTransmission(address);
	Wire.write(r);
	Wire.write(v);
	Wire.endTransmission();
}