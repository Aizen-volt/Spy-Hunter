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

void EnemyNPCCollision(enemyInfo* enemyInfo, npcInfo* npcInfo) {
	if (abs(enemyInfo->enemyPosition[Y] - npcInfo->npcPosition[Y]) < CAR_HEIGHT &&
		abs(enemyInfo->enemyPosition[X] - npcInfo->npcPosition[X]) < VERTICAL_COLLISION_OFFSET / 2) {
		if (npcInfo->startLane < 2) {
			npcInfo->npcPosition[Y]++;
			enemyInfo->enemyPosition[Y]--;
		}
		else {
			DestroyEnemy(enemyInfo, ENEMY_SHOOT);
			DestroyNpc(npcInfo);
		}
	}

	if (abs(enemyInfo->enemyPosition[Y] - npcInfo->npcPosition[Y]) < CAR_HEIGHT) {
		if (abs(enemyInfo->enemyPosition[X] - npcInfo->npcPosition[X]) < VERTICAL_COLLISION_OFFSET && enemyInfo->enemyPosition[X] > npcInfo->npcPosition[X])
			npcInfo->npcPosition[X]--;
		if (abs(enemyInfo->enemyPosition[X] - npcInfo->npcPosition[X]) < VERTICAL_COLLISION_OFFSET && enemyInfo->enemyPosition[X] < npcInfo->npcPosition[X])
			npcInfo->npcPosition[X]++;
	}
}


void Enemy2NPCCollision(enemyInfo* enemyInfo, npcInfo* npcInfo) {
	if (abs(enemyInfo->enemy2Position[Y] - npcInfo->npcPosition[Y]) < CAR_HEIGHT &&
		abs(enemyInfo->enemy2Position[X] - npcInfo->npcPosition[X]) < VERTICAL_COLLISION_OFFSET / 2) {
		if (npcInfo->startLane < 2) {
			DestroyEnemy(enemyInfo, ENEMY_FORCE_OUT);
			DestroyNpc(npcInfo);
		}
		else {
			npcInfo->npcPosition[Y]++;
			enemyInfo->enemy2Position[Y]--;
		}
	}

	if (abs(enemyInfo->enemy2Position[Y] - npcInfo->npcPosition[Y]) < CAR_HEIGHT) {
		if (abs(enemyInfo->enemy2Position[X] - npcInfo->npcPosition[X]) < VERTICAL_COLLISION_OFFSET && enemyInfo->enemy2Position[X] > npcInfo->npcPosition[X])
			npcInfo->npcPosition[X]--;
		if (abs(enemyInfo->enemy2Position[X] - npcInfo->npcPosition[X]) < VERTICAL_COLLISION_OFFSET && enemyInfo->enemy2Position[X] < npcInfo->npcPosition[X])
			npcInfo->npcPosition[X]++;
	}
}


void PlayerEnemyCollision(windowManagement* windowManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager) {
	if (abs(playerInfo->playerPosition[Y] - enemyInfo->enemyPosition[Y]) < CAR_HEIGHT &&
		abs(playerInfo->playerPosition[X] - enemyInfo->enemyPosition[X]) < VERTICAL_COLLISION_OFFSET / 2 &&
		playerInfo->playerPosition[Y] > enemyInfo->enemyPosition[Y]) {
		DestroyEnemy(enemyInfo, ENEMY_SHOOT);
		DestroyPlayer(windowManager, colors, playerInfo, npcInfo, enemyInfo, timeManager);
	}

	if (abs(playerInfo->playerPosition[Y] - enemyInfo->enemyPosition[Y]) < CAR_HEIGHT) {
		if (abs(playerInfo->playerPosition[X] - enemyInfo->enemyPosition[X]) < VERTICAL_COLLISION_OFFSET && playerInfo->playerPosition[X] > enemyInfo->enemyPosition[X]) {
			playerInfo->playerPosition[X]++;
		}
		if (abs(playerInfo->playerPosition[X] - enemyInfo->enemyPosition[X]) < VERTICAL_COLLISION_OFFSET && playerInfo->playerPosition[X] < enemyInfo->enemyPosition[X]) {
			playerInfo->playerPosition[X]--;
		}
	}
}


void PlayerEnemy2Collision(playerInfo* playerInfo, enemyInfo* enemyInfo) {
	if (abs(playerInfo->playerPosition[Y] - enemyInfo->enemy2Position[Y]) < CAR_HEIGHT &&
		abs(playerInfo->playerPosition[X] - enemyInfo->enemy2Position[X]) < VERTICAL_COLLISION_OFFSET / 2)
		enemyInfo->enemy2Position[Y]++;

	if (abs(playerInfo->playerPosition[Y] - enemyInfo->enemy2Position[Y]) < CAR_HEIGHT) {
		if (abs(playerInfo->playerPosition[X] - enemyInfo->enemy2Position[X]) < VERTICAL_COLLISION_OFFSET && playerInfo->playerPosition[X] > enemyInfo->enemy2Position[X]) {
			playerInfo->playerPosition[X]++;
			enemyInfo->enemy2Position[X] -= 2;
		}
		if (abs(playerInfo->playerPosition[X] - enemyInfo->enemy2Position[X]) < VERTICAL_COLLISION_OFFSET && playerInfo->playerPosition[X] < enemyInfo->enemy2Position[X]) {
			playerInfo->playerPosition[X]--;
			enemyInfo->enemy2Position[X] += 2;

		}
	}
}


void PlayerEnemyCollisionPlayerPush(playerInfo* playerInfo, enemyInfo* enemyInfo) {
	if (abs(playerInfo->playerPosition[Y] - enemyInfo->enemyPosition[Y]) < CAR_HEIGHT &&
		abs(playerInfo->playerPosition[X] - enemyInfo->enemyPosition[X]) < VERTICAL_COLLISION_OFFSET / 2)
		enemyInfo->enemyPosition[Y]--;

	if (abs(playerInfo->playerPosition[Y] - enemyInfo->enemyPosition[Y]) < CAR_HEIGHT) {
		if (abs(playerInfo->playerPosition[X] - enemyInfo->enemyPosition[X]) < VERTICAL_COLLISION_OFFSET && playerInfo->playerPosition[X] > enemyInfo->enemyPosition[X]) {
			enemyInfo->enemyPosition[X] -= PLAYER_POSITION_CHANGE_RATIO;
		}
		if (abs(playerInfo->playerPosition[X] - enemyInfo->enemyPosition[X]) < VERTICAL_COLLISION_OFFSET && playerInfo->playerPosition[X] < enemyInfo->enemyPosition[X]) {
			enemyInfo->enemyPosition[X] += PLAYER_POSITION_CHANGE_RATIO;
		}
	}
}


void PlayerEnemy2CollisionPlayerPush(playerInfo* playerInfo, enemyInfo* enemyInfo) {
	if (abs(playerInfo->playerPosition[Y] - enemyInfo->enemy2Position[Y]) < 95 &&
		abs(playerInfo->playerPosition[X] - enemyInfo->enemy2Position[X]) < 20)
		enemyInfo->enemy2Position[Y]++;

	if (abs(playerInfo->playerPosition[Y] - enemyInfo->enemy2Position[Y]) < 90) {
		if (abs(playerInfo->playerPosition[X] - enemyInfo->enemy2Position[X]) < 40 && playerInfo->playerPosition[X] > enemyInfo->enemy2Position[X]) {
			enemyInfo->enemy2Position[X] -= 9;
		}
		if (abs(playerInfo->playerPosition[X] - enemyInfo->enemy2Position[X]) < 40 && playerInfo->playerPosition[X] < enemyInfo->enemy2Position[X]) {
			enemyInfo->enemy2Position[X] += 9;
		}
	}
}


void EnemyEnemy2Collision(enemyInfo* enemyInfo) {
	if (abs(enemyInfo->enemyPosition[Y] - enemyInfo->enemy2Position[Y]) < CAR_HEIGHT &&
		abs(enemyInfo->enemyPosition[X] - enemyInfo->enemy2Position[X]) < VERTICAL_COLLISION_OFFSET / 2) {
		DestroyEnemy(enemyInfo, ENEMY_SHOOT);
		DestroyEnemy(enemyInfo, ENEMY_FORCE_OUT);
		return;
	}

	if (abs(enemyInfo->enemyPosition[Y] - enemyInfo->enemy2Position[Y]) < CAR_HEIGHT) {
		if (abs(enemyInfo->enemyPosition[X] - enemyInfo->enemy2Position[X]) < VERTICAL_COLLISION_OFFSET && enemyInfo->enemyPosition[X] > enemyInfo->enemy2Position[X]) {
			enemyInfo->enemyPosition[X]++;
			enemyInfo->enemy2Position[X]--;
		}
		if (abs(enemyInfo->enemyPosition[X] - enemyInfo->enemy2Position[X]) < VERTICAL_COLLISION_OFFSET && enemyInfo->enemyPosition[X] < enemyInfo->enemy2Position[X]) {
			enemyInfo->enemyPosition[X]--;
			enemyInfo->enemy2Position[X]++;
		}
	}
}


void CheckCollisionsOnMove(windowManagement* windowManager, color* colors, playerInfo* playerInfo, enemyInfo* enemyInfo, npcInfo* npcInfo, timeManagement* timeManager) {
	PlayerNPCCollision(windowManager, colors, playerInfo, npcInfo, enemyInfo, timeManager);
	PlayerEnemyCollisionPlayerPush(playerInfo, enemyInfo);
	PlayerEnemy2CollisionPlayerPush(playerInfo, enemyInfo);
	EnemyNPCCollision(enemyInfo, npcInfo);
	Enemy2NPCCollision(enemyInfo, npcInfo);
	EnemyEnemy2Collision(enemyInfo);
	CheckEnemyBeyondMap(enemyInfo, playerInfo);
	CheckEnemy2BeyondMap(enemyInfo, playerInfo);
	CheckNPCBeyondMap(npcInfo, playerInfo, timeManager);
}


void CheckCollisionsEnemies(windowManagement* windowManager, color* colors, playerInfo* playerInfo, enemyInfo* enemyInfo, npcInfo* npcInfo, timeManagement* timeManager) {
	PlayerNPCCollision(windowManager, colors, playerInfo, npcInfo, enemyInfo, timeManager);
	EnemyNPCCollision(enemyInfo, npcInfo);
	Enemy2NPCCollision(enemyInfo, npcInfo);
	PlayerEnemyCollision(windowManager, colors, playerInfo, npcInfo, enemyInfo, timeManager);
	PlayerEnemy2Collision(playerInfo, enemyInfo);
	EnemyEnemy2Collision(enemyInfo);
	CheckPlayerBeyondMap(windowManager, colors, playerInfo, npcInfo, enemyInfo, timeManager);
}


void PlayerNPCCollision(windowManagement* windowManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager) {
	if (abs(playerInfo->playerPosition[Y] - npcInfo->npcPosition[Y]) < CAR_HEIGHT &&
		abs(playerInfo->playerPosition[X] - npcInfo->npcPosition[X]) < VERTICAL_COLLISION_OFFSET / 2) {
		if (npcInfo->startLane < 2) {
			DestroyPlayer(windowManager, colors, playerInfo, npcInfo, enemyInfo, timeManager);
			DestroyNpc(npcInfo);
			FreezePointsGain(playerInfo, timeManager);
		}
		else
			npcInfo->npcPosition[Y]++;
	}

	if (abs(playerInfo->playerPosition[Y] - npcInfo->npcPosition[Y]) < CAR_HEIGHT) {
		if (abs(playerInfo->playerPosition[X] - npcInfo->npcPosition[X]) < VERTICAL_COLLISION_OFFSET && playerInfo->playerPosition[X] > npcInfo->npcPosition[X])
			npcInfo->npcPosition[X] -= PLAYER_POSITION_CHANGE_RATIO;
		if (abs(playerInfo->playerPosition[X] - npcInfo->npcPosition[X]) < VERTICAL_COLLISION_OFFSET && playerInfo->playerPosition[X] < npcInfo->npcPosition[X])
			npcInfo->npcPosition[X] += PLAYER_POSITION_CHANGE_RATIO;
	}
}


void CheckPlayerBeyondMap(windowManagement* windowManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager) {
	if (playerInfo->playerPosition[X] < FORCE_OUT_BORDER_LEFT ||
		playerInfo->playerPosition[X] > FORCE_OUT_BORDER_RIGHT)
		DestroyPlayer(windowManager, colors, playerInfo, npcInfo, enemyInfo, timeManager);
}


void CheckEnemyBeyondMap(enemyInfo* enemyInfo, playerInfo* playerInfo) {
	if (enemyInfo->enemyPosition[X] < FORCE_OUT_BORDER_LEFT ||
		enemyInfo->enemyPosition[X] > FORCE_OUT_BORDER_RIGHT) {
		DestroyEnemy(enemyInfo, ENEMY_SHOOT);
		playerInfo->userScoreTemp += SHOOT_SCORE_INCREASE_RATE;
		playerInfo->userScore = ((int)playerInfo->userScoreTemp / POINTS_MULTIPLICITY) * POINTS_MULTIPLICITY;
	}
}


void CheckEnemy2BeyondMap(enemyInfo* enemyInfo, playerInfo* playerInfo) {
	if (enemyInfo->enemy2Position[X] < FORCE_OUT_BORDER_LEFT ||
		enemyInfo->enemy2Position[X] > FORCE_OUT_BORDER_RIGHT) {
		DestroyEnemy(enemyInfo, ENEMY_FORCE_OUT);
		playerInfo->userScoreTemp += SHOOT_SCORE_INCREASE_RATE;
		playerInfo->userScore = ((int)playerInfo->userScoreTemp / POINTS_MULTIPLICITY) * POINTS_MULTIPLICITY;
	}
}


void CheckNPCBeyondMap(npcInfo* npcInfo, playerInfo* playerInfo, timeManagement* timeManager) {
	if (npcInfo->npcPosition[X] < FORCE_OUT_BORDER_LEFT ||
		npcInfo->npcPosition[X] > FORCE_OUT_BORDER_RIGHT) {
		DestroyNpc(npcInfo);
		FreezePointsGain(playerInfo, timeManager);
	}
}