#include "SD_Interface.h"
#include <stdbool.h>
#include <SD.h>
#include <SPI.h>


static bool UID_Match(SD_Struct* sd, char UID[]);
//set UID of SD_Struct
static void Set_UID(char UID[], SD_Struct* sd);
//set UID and FilePath of SD_Struct
static bool Set_Path(SD_Struct* sd, char UID[]);
//check sd card for Player with same UID 
static bool Player_Exists(char UniqueNum[]);
//Reads Money from Player File adn return is as  long
static void Read_Money(SD_Struct* sd);
//writes money on player file
static void Write_Money(SD_Struct* sd);
//search UID->UniqueNum table for the UID and set the UniqueNum of the player.
static bool SDMainDataSearch(SD_Struct *sd);


char SD_INIT(char CsPin, char MosiPin, char MisoPin, char SckPin) {

	pinMode(3, OUTPUT);

	if (SD.begin(3, 5, 6, 7)) {
		return SD_INIT_SUCCESS;
	}//end if
	else {
		return SD_INIT_FAIL;
	}//end else
}//end SD_INIT(int CD_Pin)

bool Get_SD_Struct(char UID[], SD_Struct* player) {
	
	SD_Struct* sd = player;
	//get the UniqueNum 
	if (Set_Path(sd, UID)) {
		
		if (!Player_Exists(sd->UniqueNum)) {
			
			sd->Money = 0;
							//Serial.print("creating new file: ");
							//Serial.println(sd->FilePath);
			if (Manage_Money(sd, 0, MONEY_ADD)) {
				return true;
			}
			else {
				return false;
			}

			
		}//end if (!Player_Exists(UID))
		else {
			
							//Serial.print("reading from existing file: ");
							//Serial.println(sd->FilePath);
			Get_Money(sd);
			return true;

		}//end else
	}
	return false;
}//end Create_SD_Struct()

void Delete_Player(SD_Struct* sd) {

	SD.remove(sd->FilePath);
}//end Delete_SD_Struct(SD_Struct sd)

static void Set_UID(char UID[], SD_Struct* sd) {

	for (int index = 0; index <= 4; index++) {
		sd->UID[index] = UID[index];
	}//end for

}// end Get_UID(char UID[], SD_Struct sd)

static bool Set_Path(SD_Struct* sd, char UID[]) {


	Set_UID(UID, sd);

	if (SDMainDataSearch(sd)) {
		
		sd->FilePath[0] = sd->UniqueNum[0];
		sd->FilePath[1] = sd->UniqueNum[1];
		sd->FilePath[2] = sd->UniqueNum[2];
		sd->FilePath[3] = '.';
		sd->FilePath[4] = 't';
		sd->FilePath[5] = 'x';
		sd->FilePath[6] = 't';
		sd->FilePath[7] = '\0';
		return true;
	}
	return false;
}

static bool Player_Exists(char UniqueNum[]) {
	char Path[8];
	
	Path[0] = UniqueNum[0];
	Path[1] = UniqueNum[1];
	Path[2] = UniqueNum[2];
	Path[3] = '.';
	Path[4] = 't';
	Path[5] = 'x';
	Path[6] = 't';
	Path[7] = '\0';

	if (SD.exists(Path)) {
		return true;
	}
	else {
		return false;
	}
}
//test due
bool Manage_Money(SD_Struct* sd,  long Money, char Mode) {
	//initialize and assign TempMoney
	long TempMoney = sd->Money;
	if (Mode == MONEY_ADD) {
		TempMoney = sd->Money + Money;
	}
	else {//if Mode == MONEY_SUBTRACT
		TempMoney = sd->Money - Money;
	}
	//check if TempMoney is valid
	if (TempMoney >= 0 && TempMoney < MONEY_MAX) {
		
		sd->myFile = SD.open(sd->FilePath, O_WRITE | O_CREAT | O_TRUNC);
	}
	else {
		return false;
	}
		/*if (Mode == MONEY_ADD) {
			TempMoney = sd->Money + Money;
			//Serial.print("TempMoney = ");
			//Serial.println(TempMoney);
			if (TempMoney >= 0 && TempMoney < MONEY_MAX) {
				sd->myFile = SD.open(sd->FilePath, O_WRITE | O_CREAT | O_TRUNC);
			}
			else { return false; }
		}
		else {//if Mode == MONEY_SUBTRACT
			
			TempMoney = sd->Money - Money;
				//Serial.print("TempMoney = ");
				//Serial.println(TempMoney);
			if (TempMoney >= 0 && TempMoney < MONEY_MAX) {
				sd->myFile = SD.open(sd->FilePath, O_WRITE | O_CREAT | O_TRUNC);
			}
			else {
				return false;
			}
		}//if Mode == MONEY_SUBTRACT
		*/
	//check if file opened successfuly and if so write to it
	if(sd->myFile){
		sd->Money = TempMoney;
		Write_Money(sd);
		sd->myFile.close();

		return true;
	}//end if (sd->myFile)
	else {

		return false;
	}//end else
}

static void Read_Money(SD_Struct* sd) {
	long Money = 0;
	char CurrentChar;
	
	while (sd->myFile.available()) {
		
		for (int index = 8; index >= 0; index--) {
			
			CurrentChar = sd->myFile.read();
			Money += (CurrentChar - 48) * pow(10, index);
		}//end for
	}//end while

	sd->Money = Money;
}

static void Write_Money(SD_Struct* sd) {
	long TempMoney = sd->Money;
	char CurrentDigit;
	//Serial.print("writing: ");
	for (int index = 8; index >= 0; index--) {

		CurrentDigit = TempMoney / pow(10, index);
		sd->myFile.print(CurrentDigit, DEC);
		//Serial.print(CurrentDigit, DEC);
		TempMoney -= CurrentDigit * pow(10, index);

	}//end for
	//Serial.println();
}

static bool UID_Match(SD_Struct* sd, char UID[]) {
	int match = 0;

	for (int counter = 0; counter < 4; counter++) {
		if (sd->UID[counter] == UID[counter]) {
			match++;
		}//end if
	}//end for

	if (match == 4) {
		return true;
	}//end if
	else {
		return false;
	}//end else
}

char Get_Money(SD_Struct* sd) {

	sd->myFile = SD.open(sd->FilePath, FILE_READ);

	if (sd->myFile) {
	    Read_Money(sd);
		sd->myFile.close();
		return FILE_OPEN_SECCESS;
	}
	else {

		return FILE_OPEN_FAIL;
	}
}

static bool SDMainDataSearch(SD_Struct *sd)
{
	int SimularByte = 0;

	sd->myFile = SD.open("hello.txt", FILE_READ);
	if (sd->myFile) {
		//Serial.println("hello.txt opened succesfully");
		do
		{
			SimularByte = 0;
			//Serial.print("Current UID: ");
			for (int count = 0; count < 4; count++) //read UID bytes
			{
				char a;
				a = sd->myFile.read();
				//Serial.print(a);
				if (sd->UID[count] == a)//compare UID byte[x]
				{
					SimularByte++;
				}//end if
			}//end for
			//Serial.println();
			if (SimularByte == 4)//check if UID match found
			{
				//Serial.println("UID MATCH!");
				for (int index = 0; index < 3; index++)//read and return the unique number
				{
					sd->UniqueNum[index] = sd->myFile.read();
				}//end for
				sd->myFile.close();
				return true;
			}//end if
			else //read unnessery bytes
			{
				sd->myFile.read();
				sd->myFile.read();
				sd->myFile.read();
			}
		} while (sd->myFile.available());

		sd->myFile.close();
		return false;//UID-U_Number match not found
	}

	return false;
}//end function
