/*
 Name:		MonopolyCreditCard.ino
 Created:	12/1/2016 5:53:30 PM
 Author:	guyco
*/

// the setup function runs once when you press reset or power the board


#include <LiquidCrystal_I2C.h>
#include <MFRC522\MFRC522.h>
#include <MPR121_defs.h>
#include <MPR121.h>
#include <SD.h>
#include <stdbool.h>
#include <Wire.h>
#include <SPI.h>
#include "MPR121_Interface.h"
#include "SD_Interface.h"
#include "MFRC522_Interface.h"

#define MPR121_InterruptPin 4
#define MPR121_TouchThreshold 20
#define MPR121_ReleaseThreshold 40

#define INPUT_TOO_LONG -10

void Display_Print(char* RawOne, char* RawTwo);
void GetLong();
char GetInput();
void Clear_Char_Array();
void Scroll();

char Menu_Main();

char TempCharArray[9];
char CharArray[9] = { 0 };
char CharArrayPosition = 0;

char UID[4];
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
	Display_Print("Initializing...", "Version 1.0");

	lcd.begin();
	lcd.backlight();
	Serial.begin(9600);
	while (!Serial)
	{

	}
	Clear_Char_Array();
	RC522_INIT();
	if (MPR121_INIT(MPR121_InterruptPin, MPR121_TouchThreshold, MPR121_ReleaseThreshold) == MPR121_INIT_FAIL) {
		return;
	}
	if (SD_INIT(3, 5, 6, 7) == SD_INIT_FAIL) {
		return;
	}

	Serial.println("Done Initialation");
/*
	Serial.begin(9600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}


	if (SD_INIT(3, 5, 6, 7) == SD_INIT_FAIL) {
		Serial.println("SD not Initialized");
	}
	
	
	SD_Struct player;
	SD_Struct* sd = &player;
	Serial.print("Player UID: ");
	Serial.print(UID[0]);
	Serial.print(UID[1]);
	Serial.print(UID[2]);
	Serial.println(UID[3]);
	 long Money;
		
	 if (Get_SD_Struct(UID, sd)) {

		 Money = Get_Money(sd);

		 Serial.print("Current Money = ");
		 Serial.println(Money);
		 while (Serial.available() == 0)
		 {

		 }
		 Money = Serial.parseInt();
		 if (Money == 0) {
			 Delete_Player(sd);
		 }
		 else {

			 if (Manage_Money(sd, Money, MONEY_ADD)) {
				 Money = Get_Money(sd);

				 Serial.print("Current Money after adding = ");
				 Serial.println(Money);
			 }
			 else {
				 Serial.println("error Managing Money");
			 }
		 }
		//Delete_SD_Struct(sd);
	}
	else {
		Serial.println("error creating file");
	}
	
	Serial.println("----------------------------------------------");
	*/

/*	Serial.begin(9600);
	while (!Serial)
	{

	}

	if (MPR121_INIT(4, 40, 20) != MPR121_INIT_FAIL) {
		Serial.println("success");
		while (1) {
			int electrode = Update_Touch_Status();
			if (electrode != Touch_Status_Not_Changed) {
				Serial.print("electrode: ");
				Serial.print(electrode);
				Serial.println(" was touched");
				
			}
		}
	}
	else {
		Serial.println("error");
	}*/

}

// the loop function runs over and over again until power down or reset
void loop() {
	/*Get_UID(UID);
	Serial.print("UID: ");
	for (char index = 0; index < 4; index++) {
		Serial.print(UID[index]);
	}
	Serial.println();
	Serial.println("----------------------------");*/
	char electrode = GetInput();
	lcd.clear();
	lcd.print(TempCharArray);
	if (electrode == Electrode_Enter) {
		Serial.println("========================");
	}
		delay(500);

}

void Display_Print(char* RawOne, char* RawTwo) {
	
	lcd.print(RawOne);
	lcd.setCursor(0, 1);
	lcd.print(RawTwo);

}

char GetInput() {
	char electrode = Update_Touch_Status();

		if (electrode == Touch_Status_Not_Changed) {
			return Touch_Status_Not_Changed;
		}
		else if (electrode == Electrode_Enter) {
			
			GetLong();
			Clear_Char_Array();
			return electrode;
		}
		else if (electrode == Electrode_Delete) {
			CharArrayPosition--;
			TempCharArray[CharArrayPosition] = NULL;
			return electrode;
		}
		else {
			if (CharArrayPosition != 9) {
				TempCharArray[CharArrayPosition] = electrode + 48;
				CharArrayPosition++;
				return electrode;
			}
		}

}

void GetLong() {
	char index = 0;
	Serial.println(TempCharArray);
	Serial.print("CharArray: ");
	for (char counter = (CharArrayPosition - 1); counter >= 0; counter--) {
		
		CharArray[counter] = TempCharArray[index];
		index++;
		Serial.print(CharArray[counter]);
	}
	Serial.println();
	test(CharArray);
	//Clear_Char_Array();
}

void Clear_Char_Array() {
	for (char index = 0; index < 9; index++) {
		TempCharArray[index] = NULL;
	}

	for (byte index = 0; index < 9; index++) {
		CharArray[index] = '0';
	}
	CharArrayPosition = 0;
}

void Scroll() {
	delay(2000);
	for (char count = 0; count < 40; count++) {
		lcd.scrollDisplayLeft();
		delay(550);
	}
}

char Menu_Main() {
	char electrode;
	Display_Print("1 ADD Player 2 Delete Player", "3 Manage Money 4 Transfer Money");
	delay(500);

	do {
		electrode = GetInput();
		lcd.clear();
		Display_Print("Choose Mode", TempCharArray);
		delay(200);
	} while (electrode != Electrode_Enter);
}

void Menu_Add_Player() {

}

void Menu_Delete_Player() {

}

void Menu_Manage_Money() {

}

void Menu_Transfer_Money() {

}

