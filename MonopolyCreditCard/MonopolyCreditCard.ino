/*
 Name:		MonopolyCreditCard.ino
 Created:	12/1/2016 5:53:30 PM
 Author:	guyco
*/

// the setup function runs once when you press reset or power the board


#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
//#include <MFRC522\MFRC522.h>

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
#define INPUT_NOT_VALID 24

#define SHOW_INPUT 51
#define HIDE_INPUT 50

void Display_Print(char* RawOne, char* RawTwo);
void GetLong();
char GetInput();
void Clear_Char_Array();
void Scroll();
void Update_Input(char Mode);

void Menu_Main();
void Menu_Add_Player();
void Menu_Delete_Player();
void Menu_Transfer_Money();

bool Transfer_From();
void Transfer_To();

char TempCharArray[9];
char CharArrayPosition = 0;
long Input = 0;

char UID[4];
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {


	lcd.begin();
	lcd.backlight();
	
	Clear_Char_Array();
	RC522_INIT();
	if (MPR121_INIT() == MPR121_INIT_FAIL) {
		return;
	}
	if (SD_INIT(3, 5, 6, 7) == SD_INIT_FAIL) {
		return;
	}
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
	/*char electrode = GetInput();
	lcd.clear();
	lcd.print(TempCharArray);
	if (electrode == Electrode_Enter) {
		Serial.print("CharArray: ");
		Serial.println(TempCharArray);
		Serial.print("input: ");
		Serial.println(Input);
		Serial.println("========================");
	}
		delay(500);
		*/

	Menu_Main();
	if (Input == 1) {
		Menu_Add_Player();
	}
	else if (Input == 2) {
		Menu_Delete_Player();
	}
	else if (Input == 3) {
		Menu_Transfer_Money();
	}
	
	/*if (Input == 1) {
		Display_Print("please scan", "player card");
		Get_UID(UID);
		Display_Print("UID Scanned", "");
		delay(1000);
		if (Get_SD_Struct(UID, &sd)) {
			Display_Print("player created", "Have fun");
			delay(2000);
		}
		else {
			Display_Print("Error creating", "player");
			delay(2000);
		}
	}*/
}

void Display_Print(char* RawOne, char* RawTwo) {
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(RawOne);
	lcd.setCursor(0, 1);
	lcd.print(RawTwo);
	
}

char GetInput() {
	char electrode = readTouchInputs();

		if (electrode == Touch_Status_Not_Changed) {
			return Touch_Status_Not_Changed;
		}
		else if (electrode == Electrode_Enter) {
			
			GetLong();
			Clear_Char_Array();
			return electrode;
		}
		else if (electrode == Electrode_Delete) {
			if (CharArrayPosition > 0) {
			--CharArrayPosition;
			TempCharArray[CharArrayPosition] = NULL;
			return electrode;
			}
		}
		else {
			if (CharArrayPosition != 8) {
				TempCharArray[CharArrayPosition] = electrode + 48;
				CharArrayPosition++;
				return electrode;
			}
		}

}

void GetLong() {
	
	char counter = CharArrayPosition - 1;
	Input = 0;
	//Serial.print("counter: ");
	//Serial.println(counter, DEC);
	for (char index = 0; index <= (CharArrayPosition - 1); index++) {
		//Serial.print("Current Char: ");
		//Serial.println(TempCharArray[index]);
		//Serial.print("index: ");
		//Serial.println(index, DEC);
		Input += (TempCharArray[index] - 48) * pow(10, counter);
		counter--;
	}
}

void Update_Input(char Mode) {
	char electrode;
	if (Mode == SHOW_INPUT) {
		do {
			electrode = GetInput();
			lcd.setCursor((6 + CharArrayPosition), 1);
			lcd.print(" ");
			lcd.setCursor(0, 1);
			lcd.print("Input:");
			lcd.print(TempCharArray);
			delay(50);

		} while (electrode != Electrode_Enter);
		lcd.home();
	}//end Mode == SHOW_INPUT
	else {
		do {
			electrode = readTouchInputs();
		}while (electrode == Touch_Status_Not_Changed);
		Input = electrode;
	}//Mode == HIDE_INPUT
}

void Clear_Char_Array() {
	for (char index = 0; index < 9; index++) {
		TempCharArray[index] = NULL;
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

void Menu_Main() {
	char electrode;
	Display_Print("1)Add 2)Delete", "3)Transfer Money");
	//Scroll();

	Update_Input(HIDE_INPUT);

}

void Menu_Add_Player() {
	SD_Struct player;
	SD_Struct *sd = &player;
	char MoneyMode;

		Display_Print("Manage Money", "pls scan card");
		Get_UID(UID);
		lcd.print("OK");
		delay(1000);
		if (Get_SD_Struct(UID, sd)) {
			//set MoneyMode return if invalid
			Display_Print("1)Add 2)Sub", "Now:");
			lcd.print(sd->Money);
			
			Update_Input(HIDE_INPUT);
			if (Input == 1) {
				MoneyMode = MONEY_ADD;
				Display_Print("how much to add?", "");

			}
			else if (Input == 2) {
				MoneyMode = MONEY_SUB;
				Display_Print("how much to sub", "");
			}
			else {
				return;
			}
			//get amout of money and add/sub it from player file
			Update_Input(SHOW_INPUT);
			if (Manage_Money(sd, Input, MoneyMode)) {
				Display_Print("You now have:", "");
				lcd.print(sd->Money);
				delay(2000);
			}//end if
			else {
				Display_Print("ERROR:", "Manage_Money");
				delay(2000);
			}
		}//end if (Get_SD_Struct(UID, sd))
		else {
			Display_Print("ERROR:", "Get_SD_Struct");
			delay(2000);
		}
}

void Menu_Delete_Player() {
	Display_Print("Delete Player?", "1)Yes 2)No");
	Update_Input(HIDE_INPUT);
	
	if (Input == 1) {
		SD_Struct player;
		SD_Struct *sd = &player;
		//get UID
		Display_Print("Deleting Player", "pls scan card");
		Get_UID(UID);
		//Get the struct and delete the player's file
		if (Get_SD_Struct(UID, sd)) {
			Delete_Player(sd);
			Display_Print("player deleted", "");
		}//if (Get_SD_Struct(UID, sd))
		else {
			Display_Print("ERROR:", "Get_SD_Struct()");
		}
	}//if (Input == 1)


	delay(2000);
}//Menu_Delete_Player()

void Menu_Transfer_Money() {
	
	Display_Print("Transfer Money", "1)OK 2)Cancel");
	Update_Input(HIDE_INPUT);
	
	if (Input == 1) {
		
		if (Transfer_From()) {
			
			Transfer_To();
		}//if (Transfer_From())
	}// if (Input == 1)

}

bool Transfer_From() {
	SD_Struct player;
	SD_Struct *sd = &player;

	Display_Print("please scan", "first card");
	Get_UID(UID);
	if (Get_SD_Struct(UID, sd)) {
		Display_Print("You now have:", "");
		lcd.print(sd->Money);
		Update_Input(HIDE_INPUT);
		Display_Print("Trasfer Amount?", "");
		Update_Input(SHOW_INPUT);

		if (Manage_Money(sd, Input, MONEY_SUB)) {
			Display_Print("You now have:", "");
			lcd.print(sd->Money);
			delay(2000);
			return true;
		}//if (Manage_Money(sd, Input, MONEY_SUB))
		else {
			Display_Print("you dont have", "enough Money");
			delay(2000);
			return false;
		}

	}//if (Get_SD_Struct(UID, sd))
	else {
		Display_Print("ERROR:", "Get_SD_Struct()");
		delay(2000);
		return false;
	}
}

void Transfer_To() {
	SD_Struct player;
	SD_Struct *sd = &player;

	Display_Print("please scan", "second card");

	Get_UID(UID);
	if (Get_SD_Struct(UID, sd)) {
		
		if (Manage_Money(sd, Input, MONEY_ADD)) {
			
			Display_Print("Transfer", "  Completed");
			delay(1500);
			Display_Print("You now Have:", "");
			lcd.print(sd->Money);
			delay(2000);
		}//if (Manage_Money(sd, Input, MONEY_ADD))
		else {
			Display_Print("ERROR:", "Manage_Money()");
			delay(2000);
		}

	}//if (Get_SD_Struct(UID, sd))
	else {
		Display_Print("ERROR:", "Get_SD_Struct()");
	}

}