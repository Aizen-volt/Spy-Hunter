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

void InitializeTimeData(timeManagement* timeManager) {
	timeManager->tick1 = SDL_GetTicks();
	timeManager->frames = 0;
	timeManager->fpsTimer = 0;
	timeManager->fps = 0;
	timeManager->worldTime = 0;
	timeManager->distance = 0;
	timeManager->gameSpeed = DEFAULT_GAME_SPEED;
	timeManager->tick1WithoutPause = 0;
}


void CalculateTimeData(timeManagement* timeManager) {
	timeManager->tick2 = SDL_GetTicks() - timeManager->tick1WithoutPause;
	timeManager->deltaTime = (timeManager->tick2 - timeManager->tick1) * SECONDS_TO_MILISECONDS;
	timeManager->tick1 = timeManager->tick2;
	timeManager->worldTime += timeManager->deltaTime;
	timeManager->distance += timeManager->gameSpeed * timeManager->deltaTime;

	if (timeManager->gameSpeed < SECOND)
		timeManager->gameSpeed += timeManager->worldTime / SECOND;

	timeManager->fpsTimer += timeManager->deltaTime;
	if (timeManager->fpsTimer > SECONDS_BETWEEN_REFRESH) {
		timeManager->fps = timeManager->frames * 2;
		timeManager->frames = 0;
		timeManager->fpsTimer -= 0.5;
	};
}