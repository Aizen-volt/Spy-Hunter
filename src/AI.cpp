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

void RandomizeNPCAppearance(npcInfo* npcInfo) {
	srand(time(NULL));
	int appearanceSeed = rand() % 400;
	if (appearanceSeed < 250 || appearanceSeed > 300) {
		npcInfo->npcPresentOnBoard = true;
		npcInfo->npcHP = CARS_HP;
		bool laneFound = false;
		while (!laneFound) {
			int laneSeed = rand() % LANE_CHOICE_RAND;
			switch (laneSeed)
			{
			case 0:
				laneFound = true;
				npcInfo->startLane = 0;
				npcInfo->npcPosition[X] = LANE_0_POSITION;
				break;
			case 1:
				laneFound = true;
				npcInfo->startLane = 1;
				npcInfo->npcPosition[X] = LANE_1_POSITION;
				break;
			case 2:
				laneFound = true;
				npcInfo->startLane = 2;
				npcInfo->npcPosition[X] = LANE_2_POSITION;
				break;
			case 3:
				laneFound = true;
				npcInfo->startLane = 3;
				npcInfo->npcPosition[X] = LANE_3_POSITION;
				break;
			}
		}
		if (npcInfo->startLane > 1)
			npcInfo->npcPosition[Y] = CAR_SPAWN_BOTTOM;
		else
			npcInfo->npcPosition[Y] = CAR_SPAWN_TOP;
	}
}


void MoveNPC(npcInfo* npcInfo, timeManagement* timeManager) {
	npcInfo->startLane > 1 ? npcInfo->npcPosition[Y] -= NPC_POSITION_CHANGE_RATIO_Y : npcInfo->npcPosition[Y] += NPC_POSITION_CHANGE_RATIO_Y;
	if ((npcInfo->startLane < 2 && npcInfo->npcPosition[Y] > SCREEN_BORDER_BOTTOM) || (npcInfo->startLane > 1 && npcInfo->npcPosition[Y] < SCREEN_BORDER_TOP - 120)) {
		npcInfo->npcPresentOnBoard = false;
	}
}


void HandleNPC(npcInfo* npcInfo, timeManagement* timeManager) {
	if (!npcInfo->npcPresentOnBoard)
		RandomizeNPCAppearance(npcInfo);
	MoveNPC(npcInfo, timeManager);
}


void RandomizeEnemyAppearance(enemyInfo* enemyInfo, npcInfo* npcInfo, int enemyType) {
	srand(time(NULL));
	int appearanceSeed = rand() % 100;
	if (appearanceSeed < 75) {
		enemyType == ENEMY_SHOOT ? enemyInfo->enemyHP = CARS_HP : enemyInfo->enemy2HP = CARS_HP;
		enemyInfo->enemyPresentOnBoard[enemyType] = true;
		enemyType == ENEMY_SHOOT ? enemyInfo->enemyPosition[Y] = CAR_SPAWN_TOP : enemyInfo->enemy2Position[Y] = CAR_SPAWN_BOTTOM;
		bool laneFound = false;
		while (!laneFound) {
			int laneSeed = rand() % 4;
			switch (laneSeed)
			{
			case 0:
				enemyType == ENEMY_SHOOT ? enemyInfo->enemyPosition[X] = LANE_0_POSITION : enemyInfo->enemy2Position[X] = LANE_0_POSITION;
				if (abs(enemyInfo->enemyPosition[X] - enemyInfo->enemy2Position[X]) < HORIZONTAL_COLLISION_OFFSET &&
					abs(enemyInfo->enemyPosition[Y] - enemyInfo->enemy2Position[Y]) < VERTICAL_COLLISION_OFFSET) {
					enemyInfo->enemyPresentOnBoard[enemyType] = false;
					return;
				}
				enemyType == ENEMY_SHOOT ? enemyInfo->enemyStartLane = 0 : enemyInfo->enemy2StartLane = 0;
				laneFound = true;
				break;
			case 1:
				enemyType == ENEMY_SHOOT ? enemyInfo->enemyPosition[X] = LANE_1_POSITION : enemyInfo->enemy2Position[X] = LANE_1_POSITION;
				if (abs(enemyInfo->enemyPosition[X] - enemyInfo->enemy2Position[X]) < HORIZONTAL_COLLISION_OFFSET &&
					abs(enemyInfo->enemyPosition[Y] - enemyInfo->enemy2Position[Y]) < VERTICAL_COLLISION_OFFSET) {
					enemyInfo->enemyPresentOnBoard[enemyType] = false;
					return;
				}
				enemyType == ENEMY_SHOOT ? enemyInfo->enemyStartLane = 1 : enemyInfo->enemy2StartLane = 1;
				laneFound = true;
				break;
			case 2:
				enemyType == ENEMY_SHOOT ? enemyInfo->enemyPosition[X] = LANE_2_POSITION : enemyInfo->enemy2Position[X] = LANE_2_POSITION;
				if (abs(enemyInfo->enemyPosition[X] - enemyInfo->enemy2Position[X]) < HORIZONTAL_COLLISION_OFFSET &&
					abs(enemyInfo->enemyPosition[Y] - enemyInfo->enemy2Position[Y]) < VERTICAL_COLLISION_OFFSET) {
					enemyInfo->enemyPresentOnBoard[enemyType] = false;
					return;
				}
				enemyType == ENEMY_SHOOT ? enemyInfo->enemyStartLane = 2 : enemyInfo->enemy2StartLane = 2;
				laneFound = true;
				break;
			case 3:
				enemyType == ENEMY_SHOOT ? enemyInfo->enemyPosition[X] = LANE_3_POSITION : enemyInfo->enemy2Position[X] = LANE_3_POSITION;
				if (abs(enemyInfo->enemyPosition[X] - enemyInfo->enemy2Position[X]) < HORIZONTAL_COLLISION_OFFSET &&
					abs(enemyInfo->enemyPosition[Y] - enemyInfo->enemy2Position[Y]) < VERTICAL_COLLISION_OFFSET) {
					enemyInfo->enemyPresentOnBoard[enemyType] = false;
					return;
				}
				enemyType == ENEMY_SHOOT ? enemyInfo->enemyStartLane = 3 : enemyInfo->enemy2StartLane = 3;
				laneFound = true;
				break;
			}
		}
	}
}


void MoveEnemies(windowManagement* windowManager, color* colors, playerInfo* playerInfo, enemyInfo* enemyInfo, npcInfo* npcInfo, double distance, timeManagement* timeManager) {
	enemyInfo->enemyPosition[Y] += ENEMY_POSITION_CHANGE_RATIO_Y;
	enemyInfo->enemy2Position[Y] -= ENEMY2_POSITION_CHANGE_RATIO_Y;

	enemyInfo->enemyPosition[X] > playerInfo->playerPosition[X] ? enemyInfo->enemyPosition[X] -= ENEMY_POSITION_CHANGE_RATIO_X : enemyInfo->enemyPosition[X] += ENEMY_POSITION_CHANGE_RATIO_X;
	if ((abs(playerInfo->playerPosition[X] - enemyInfo->enemy2Position[X]) > VERTICAL_COLLISION_OFFSET || abs(playerInfo->playerPosition[Y] - enemyInfo->enemy2Position[Y]) < CAR_HEIGHT) &&
		playerInfo->playerPosition[Y] < enemyInfo->enemy2Position[Y])
		enemyInfo->enemy2Position[X] > playerInfo->playerPosition[X] ? enemyInfo->enemy2Position[X] -= ENEMY2_POSITION_CHANGE_RATIO_X : enemyInfo->enemy2Position[X] += ENEMY2_POSITION_CHANGE_RATIO_X;
	CheckCollisionsEnemies(windowManager, colors, playerInfo, enemyInfo, npcInfo, timeManager);

	if (enemyInfo->enemyPosition[Y] > SCREEN_BORDER_BOTTOM) {
		enemyInfo->enemyPresentOnBoard[ENEMY_SHOOT] = false;
	}
	else if (enemyInfo->enemy2Position[Y] < SCREEN_BORDER_TOP) {
		enemyInfo->enemyPresentOnBoard[ENEMY_FORCE_OUT] = false;
	}
}


void HandleEnemy(windowManagement* windowManager, color* colors, playerInfo* playerInfo, enemyInfo* enemyInfo, npcInfo* npcInfo, double distance, timeManagement* timeManager) {
	if (!enemyInfo->enemyPresentOnBoard[ENEMY_SHOOT] && (int)timeManager->worldTime % 2 == 0)
		RandomizeEnemyAppearance(enemyInfo, npcInfo, ENEMY_SHOOT);
	if (!enemyInfo->enemyPresentOnBoard[ENEMY_FORCE_OUT])
		RandomizeEnemyAppearance(enemyInfo, npcInfo, ENEMY_FORCE_OUT && (int)timeManager->worldTime % 3 == 0);
	MoveEnemies(windowManager, colors, playerInfo, enemyInfo, npcInfo, distance, timeManager);
}