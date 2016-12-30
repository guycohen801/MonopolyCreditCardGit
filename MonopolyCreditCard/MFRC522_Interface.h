#pragma once
#ifndef MFRC522_INTERFACE_H
#define MFRC522_INTERFACE_H

#define RST_PIN 9
#define SS_PIN 10


//initialize mfrc522 ---- also SPI.begin()
void RC522_INIT();

//get UID of the card
void Get_UID(char* CurrentUID);

//char UID_COMPARE(char* UID, char* TempUID);

#endif // !MFRC522_INTERFACE_H
