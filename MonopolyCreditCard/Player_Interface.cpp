#include "Player_Interface.h"
#include "stdbool.h"
#include "stdlib.h"


struct SD_Interface {

	int Size;
	int MaxSize;
	Player_Struct* PlayerArray;
};

struct Player_Interface {
	char UID[4];
	unsigned long Money;
};

static bool Player_Exists(char UID[]);



SD_Struct Create_SD_Struct() {

	SD_Struct sd = (SD_Struct) malloc(sizeof(*sd));

		if (sd == NULL) {
			return NULL;
		}

		sd->Size = INIT_SIZE_OF_PLAYER_ARRAY;
		sd->MaxSize = INIT_SIZE_OF_PLAYER_ARRAY;
		sd->PlayerArray = (Player_Struct*) malloc((sd->Size) * sizeof(**(sd->PlayerArray)));

		if (sd->PlayerArray == NULL) {
			free(sd->PlayerArray);
			return NULL;
		}

}//end Create_SD_Struct()

void Delete_SD_Struct(SD_Struct sd) {

	if (sd == NULL) {
		return;
	}
	else {
		free(sd);
	}
}//end Delete_SD_Struct(SD_Struct sd)

static void Set_UID( SD_Struct sd, char UID[]) {



}// end Get_UID(char UID[], SD_Struct sd)

bool Player_Exists(char UID[]) {

}