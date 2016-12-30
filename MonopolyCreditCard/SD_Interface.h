#pragma once
#ifndef SD_INTERFACE_H
#define SD_INTERFACE_H

#define SD_INIT_FAIL -1
#define SD_INIT_SUCCESS 1

#define PATH_CREATED 2
#define PATH_EXISTS 3
#define CURRENT_PATH 4

#define MONEY_ADD 5
#define MONEY_SUB 6

#define FILE_OPEN_FAIL 7
#define FILE_OPEN_SECCESS 8

#define MONEY_MAX 2147483647
#define MONEY_MIN -2147483647 

#include <SD.h>

struct SD_Interface {
	File myFile;
	char UID[4];
	char UniqueNum[3];
	char FilePath[8];
	unsigned long Money;
};

typedef struct SD_Interface SD_Struct;

//init SD card reader
char SD_INIT(char CsPin, char MosiPin, char MisoPin, char SckPin);

//create sd struct
bool Get_SD_Struct( char UID[], SD_Struct* player);

//delete player file
void Delete_Player(SD_Struct* sd);

//add money to player
bool Manage_Money(SD_Struct* sd, long Money, char Mode);

//read money from player file and return it as unsigned long
char Get_Money(SD_Struct* sd);


//check if playesr exists
// bool Player_Exists(char UID[]);

//Get UID
// void Set_UID(char UID[], SD_Struct sd);

//set the path of the file and UID
// int Set_Path(SD_Struct sd, char UID[])

//read the money in file and return in unsigned long
// unsigned long Read_Money(File myFile)

//check if UID is identical
// bool UID_Match(SD_Struct sd, char UID[])

#endif // !SD_INTERFACE_H

/*
SD initialize
create struct
delete struct
add money
subtract money
show money
player exists
UID match
*/