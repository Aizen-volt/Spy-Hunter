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

void NewGameCreatePlayerData(playerInfo* playerInfo) {
	playerInfo->userScore = 0;
	playerInfo->userScoreTemp = 0;
	playerInfo->scoreCanBeIncreased = true;
	playerInfo->hp = MAX_HP;
	playerInfo->playerPosition[X] = PLAYER_DEFAULT_POSITION_X;
	playerInfo->playerPosition[Y] = PLAYER_DEFAULT_POSITION_Y;
	playerInfo->weaponMode = BULLET;
	playerInfo->ammo = 0;
	playerInfo->lastHPIncrease = 0;
}


void NewGameCreateNPCData(npcInfo* npcInfo) {
	npcInfo->npcPresentOnBoard = false;
	npcInfo->npcPosition[Y] = CAR_SPAWN_TOP;
}


void NewGameCreateEnemyData(enemyInfo* enemyInfo) {
	enemyInfo->enemyPresentOnBoard[ENEMY_SHOOT] = false;
	enemyInfo->enemyPosition[Y] = CAR_SPAWN_TOP;
	enemyInfo->enemyPosition[X] = LANE_0_POSITION;
	enemyInfo->enemyPresentOnBoard[ENEMY_FORCE_OUT] = false;
	enemyInfo->enemy2Position[Y] = CAR_SPAWN_TOP;
	enemyInfo->enemy2Position[X] = LANE_1_POSITION;
}


void NewGameCreateData(timeManagement* timeManager, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, weapons* weapon) {
	InitializeTimeData(timeManager);
	NewGameCreatePlayerData(playerInfo);
	NewGameCreateNPCData(npcInfo);
	NewGameCreateEnemyData(enemyInfo);
	NewGameCreateWeaponData(weapon);
}


void NewGameCreateWeaponData(weapons* weapon) {
	weapon->onBoard = false;
	weapon->velocity = VELOCITY_BULLET;
}


void NewGame(windowManagement* windowManager, roadBMPs* roadBMPs, carsBMPs* carsBMPs, weaponsBMPs* weaponsBMPs, weapons* weapon, color* colors, timeManagement* timeManager, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, powerUp* powerUp) {
	PrepareGame(windowManager);
	LoadRoadBMPs(windowManager, roadBMPs);
	LoadCarsBMPs(windowManager, carsBMPs);
	LoadWeaponsBMPs(windowManager, weaponsBMPs);
	LoadRGBMaps(windowManager->screen, colors);
	NewGameCreateData(timeManager, playerInfo, npcInfo, enemyInfo, weapon);
	powerUp->onBoard = false;
}