#include <MFRC522.h>

#include "MFRC522_Interface.h"


MFRC522 mfrc522(SS_PIN, RST_PIN);

void RC522_INIT() {

	SPI.begin();
	mfrc522.PCD_Init();
}


void Get_UID(char* CurrentUID) {

	while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
		delay(50);
	}

	// Dump UID
	for (char i = 0; i < mfrc522.uid.size; i++) {

		CurrentUID[i] = mfrc522.uid.uidByte[i];
	}//end For
}
