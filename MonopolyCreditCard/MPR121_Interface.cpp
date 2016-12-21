
#include "MPR121_Interface.h"
#include <MPR121.h>
#include <Wire.h>




//Initialize the MPR121
char MPR121_INIT(char SetInterrupPin, char SetTouchThreshold, char SetReleaseThreshold) {
	
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
}//end MPR121_INIT

//check if an electride was touched and return it's number
char Update_Touch_Status() {
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
