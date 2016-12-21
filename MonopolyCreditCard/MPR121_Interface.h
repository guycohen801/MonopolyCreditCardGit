// MPR121_Interface.h

#ifndef MPR121_INTERFACE_h
#define MPR121_INTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#endif

#define MPR121_INIT_FAIL -1
#define MPR121_INIT_SUCCESS 1
#define NumOfElectrodes 12
#define Touch_Status_Not_Changed -12

#define Electrode_0 0
#define Electrode_1 1
#define Electrode_2 2
#define Electrode_3 3
#define Electrode_4 4
#define Electrode_5 5
#define Electrode_6 6
#define Electrode_7 7
#define Electrode_8 8
#define Electrode_9 9
#define Electrode_Delete 10
#define Electrode_Enter 11

typedef struct MPR121_Interface* MPR121_Struct;

//Initialize the MPR121
char MPR121_INIT(char SetInterrupPin, char SetTouchThreshold, char SetReleaseThreshold);

//check if an electride was touched and return it's number
char Update_Touch_Status();

#endif