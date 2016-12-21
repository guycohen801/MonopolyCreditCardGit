#pragma once
#ifndef PLAYER_INTERFACE_H
#define PLAYER_INTERFACE_H

#define INIT_SIZE_OF_PLAYER_ARRAY 2


typedef struct SD_Interface* SD_Struct;
typedef struct Player_Interface* Player_Struct;

SD_Struct Create_SD_Struct();

void Delete_SD_Struct(SD_Struct sd);

#endif // !PLAYER_INTERFACE_H
