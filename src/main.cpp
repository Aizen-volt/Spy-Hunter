#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES

#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include <io.h>

#include "Config.h"
#include "Structs.h"

#include "AI.h"
#include "Collisions.h"
#include "DrawFunctions.h"
#include "FileManagement.h"
#include "GameEvents.h"
#include "NewGame.h"
#include "SDLManagement.h"
#include "TimeManagement.h"
#include "UserInput.h"


extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char** argv) {
	timeManagement timeManager;
	windowManagement windowManager;
	roadBMPs roadBMPs;
	carsBMPs carsBMPs;
	weaponsBMPs weaponsBMPs;
	weapons weapon;
	color colors;
	playerInfo playerInfo;
	npcInfo npcInfo;
	enemyInfo enemyInfo;
	powerUp powerUp;

	//to prevent overlapping of spawning cars
	bool lanesOccupied[4] = {};

	//used for creating infinite map effect - sprites reset their y-axis position after fixed time
	double bmpsOffset;

	NewGame(&windowManager, &roadBMPs, &carsBMPs, &weaponsBMPs, &weapon, &colors, &timeManager, &playerInfo, &npcInfo, &enemyInfo, &powerUp);

	while (true) {
		bmpsOffset = BMPS_OFFSET_FROM_DISTANCE;

		HoldGameFrame(&timeManager, &windowManager, &colors, &roadBMPs, &carsBMPs, &weaponsBMPs, &weapon, bmpsOffset, &playerInfo, &enemyInfo, &npcInfo, &powerUp);
	};

	EndProgram(&windowManager);
};
