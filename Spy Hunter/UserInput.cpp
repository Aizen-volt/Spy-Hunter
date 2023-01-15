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

void DispatchInputFunctions(windowManagement* windowManager, timeManagement* timeManager, playerInfo* playerInfo, color* colors, enemyInfo* enemyInfo, npcInfo* npcInfo, weapons* weapon, powerUp* powerUp) {

	while (SDL_PollEvent(&windowManager->event))
		if (windowManager->event.type == SDL_QUIT)
			EndProgram(windowManager);

	SDL_Delay(10);

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	if (currentKeyStates[SDL_SCANCODE_ESCAPE])
		EndProgram(windowManager);

	if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
		PlayerMove(playerInfo, PLAYER_POSITION_CHANGE_RATIO, 0);
		CheckCollisionsOnMove(windowManager, colors, playerInfo, enemyInfo, npcInfo, timeManager);
	}

	if (currentKeyStates[SDL_SCANCODE_LEFT]) {
		PlayerMove(playerInfo, -PLAYER_POSITION_CHANGE_RATIO, 0);
		CheckCollisionsOnMove(windowManager, colors, playerInfo, enemyInfo, npcInfo, timeManager);
	}

	if (currentKeyStates[SDL_SCANCODE_UP]) {
		PlayerMove(playerInfo, 0, -PLAYER_POSITION_CHANGE_RATIO);
		CheckCollisionsOnMove(windowManager, colors, playerInfo, enemyInfo, npcInfo, timeManager);
	}

	if (currentKeyStates[SDL_SCANCODE_DOWN]) {
		PlayerMove(playerInfo, 0, PLAYER_POSITION_CHANGE_RATIO);
		CheckCollisionsOnMove(windowManager, colors, playerInfo, enemyInfo, npcInfo, timeManager);
	}

	if (currentKeyStates[SDL_SCANCODE_SPACE])
		if (!weapon->onBoard)
			PlayerShoot(playerInfo, weapon);

	if (currentKeyStates[SDL_SCANCODE_N])
		NewGameCreateData(timeManager, playerInfo, npcInfo, enemyInfo, weapon);

	if (currentKeyStates[SDL_SCANCODE_P])
		PauseGame(windowManager, colors, timeManager);

	if (currentKeyStates[SDL_SCANCODE_S])
		SaveGame(windowManager, colors, playerInfo, npcInfo, enemyInfo, timeManager, weapon, powerUp);

	if (currentKeyStates[SDL_SCANCODE_L])
		LoadGame(windowManager, timeManager, colors, playerInfo, npcInfo, enemyInfo, weapon, powerUp);

	if (currentKeyStates[SDL_SCANCODE_T])
		DisplayScoreList(windowManager, colors, timeManager);

	if (currentKeyStates[SDL_SCANCODE_F])
		FinishGame(windowManager, colors, timeManager, playerInfo);

	SDL_PumpEvents();
}