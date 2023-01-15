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

void HoldGameFrame(timeManagement* timeManager, windowManagement* windowManager, color* colors, roadBMPs* roadBMPs, carsBMPs* carsBMPs, weaponsBMPs* weaponsBMPs, weapons* weapon, double bmpsOffset, playerInfo* playerInfo, enemyInfo* enemyInfo, npcInfo* npcInfo, powerUp* powerUp) {

	DispatchInputFunctions(windowManager, timeManager, playerInfo, colors, enemyInfo, npcInfo, weapon, powerUp);
	CalculateWeaponData(weapon, playerInfo, npcInfo, enemyInfo, timeManager);
	CheckCollisionsEnemies(windowManager, colors, playerInfo, enemyInfo, npcInfo, timeManager);

	SDL_FillRect(windowManager->screen, NULL, colors->black);
	DrawMapBMPs(windowManager->screen, roadBMPs, bmpsOffset);
	DrawCars(windowManager->screen, carsBMPs, playerInfo, npcInfo, enemyInfo);
	DrawWeapons(windowManager->screen, weaponsBMPs, weapon);
	DrawPowerUp(windowManager->screen, weaponsBMPs, powerUp);
	DrawLegends(windowManager->screen, colors, windowManager->charset, roadBMPs->heart, timeManager->worldTime, playerInfo->userScore, playerInfo->hp, playerInfo->ammo);

	UpdateScreenOutput(windowManager);

	CalculateTimeData(timeManager);

	HandleEnemy(windowManager, colors, playerInfo, enemyInfo, npcInfo, timeManager->distance, timeManager);
	HandleNPC(npcInfo, timeManager);

	HandlePowerUp(powerUp, playerInfo);

	if (playerInfo->hp < MAX_HP)
		CheckHPIncreaseConditions(playerInfo);


	CheckScoreIncreaseConditions(playerInfo, timeManager);

	if (playerInfo->scoreCanBeIncreased) {
		playerInfo->userScoreTemp += SCORE_INCREASE_RATE * timeManager->deltaTime;
		playerInfo->userScore = ((int)playerInfo->userScoreTemp / POINTS_MULTIPLICITY) * POINTS_MULTIPLICITY;
	}
	timeManager->frames++;
}


void PauseGame(windowManagement* windowManager, color* colors, timeManagement* timeManager) {
	while (true) {
		timeManager->tick1WithoutPause = SDL_GetTicks() - timeManager->tick2;
		DisplayPauseScreen(windowManager->screen, colors, windowManager->charset);
		UpdateScreenOutput(windowManager);
		SDL_Delay(3 * INPUT_DELAY);
		const Uint8* state = SDL_GetKeyboardState(NULL);
		SDL_PumpEvents();
		if (state[SDL_SCANCODE_P])
			return;
	}
}


void ResetMap(playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo) {
	playerInfo->playerPosition[X] = PLAYER_DEFAULT_POSITION_X;
	playerInfo->playerPosition[Y] = PLAYER_DEFAULT_POSITION_Y;
	NewGameCreateNPCData(npcInfo);
	NewGameCreateEnemyData(enemyInfo);
}


void FinishGame(windowManagement* windowManager, color* colors, timeManagement* timeManager, playerInfo* playerInfo) {
	while (true) {
		DisplayGameFinishedScreen(windowManager, colors, timeManager->worldTime, playerInfo->userScore);
		UpdateScreenOutput(windowManager);
		SDL_Delay(INPUT_DELAY);
		const Uint8* state = SDL_GetKeyboardState(NULL);
		SDL_PumpEvents();
		if (state[SDL_SCANCODE_Y]) {
			AddScoreToList(playerInfo->userScore, timeManager->worldTime);
			break;
		}
		if (state[SDL_SCANCODE_N])
			break;
	}
	EndProgram(windowManager);
}


void DestroyPlayer(windowManagement* windowManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager) {
	if (timeManager->worldTime < IMMORTALITY_TIME) {
		ResetMap(playerInfo, npcInfo, enemyInfo);
		return;
	}
	timeManager->tick1WithoutPause += 10 * INPUT_DELAY;
	playerInfo->hp--;
	SDL_Delay(10 * INPUT_DELAY);
	SDL_PumpEvents();
	ResetMap(playerInfo, npcInfo, enemyInfo);
	if (playerInfo->hp <= 0)
		FinishGame(windowManager, colors, timeManager, playerInfo);

}


void DestroyEnemy(enemyInfo* enemyInfo, int enemyType) {
	enemyInfo->enemyPresentOnBoard[enemyType] = false;
	enemyType == ENEMY_SHOOT ? enemyInfo->enemyPosition[X] = LANE_0_POSITION : enemyInfo->enemy2Position[X] = LANE_0_POSITION;
	enemyType == ENEMY_SHOOT ? enemyInfo->enemyPosition[Y] = SCREEN_HEIGHT + 200 : enemyInfo->enemy2Position[Y] = -200;
}


void DestroyNpc(npcInfo* npcInfo) {
	npcInfo->npcPresentOnBoard = false;
	npcInfo->startLane < 2 ? npcInfo->npcPosition[Y] = 0 : npcInfo->npcPosition[Y] = CAR_SPAWN_BOTTOM;

}


void FreezePointsGain(playerInfo* playerInfo, timeManagement* timeManager) {
	playerInfo->scoreCanBeIncreased = false;
	playerInfo->lastScoreRestrictionTime = timeManager->worldTime;
}


void PlayerMove(playerInfo* playerInfo, int x, int y) {
	playerInfo->playerPosition[X] += x;
	playerInfo->playerPosition[Y] += y;

	if (playerInfo->playerPosition[X] > SCREEN_BORDER_RIGHT)
		playerInfo->playerPosition[X] -= x;
	if (playerInfo->playerPosition[X] < SCREEN_BORDER_LEFT)
		playerInfo->playerPosition[X] -= x;
	if (playerInfo->playerPosition[Y] < SCREEN_BORDER_TOP)
		playerInfo->playerPosition[Y] -= y;
	if (playerInfo->playerPosition[Y] > SCREEN_PLAYER_BORDER_BOTTOM)
		playerInfo->playerPosition[Y] -= y;
}


void CheckScoreIncreaseConditions(playerInfo* playerInfo, timeManagement* timeManager) {
	if (playerInfo->playerPosition[X] > ROAD_BORDER_RIGHT ||
		playerInfo->playerPosition[X] < ROAD_BORDER_LEFT) {
		playerInfo->scoreCanBeIncreased = false;
		return;
	}
	if (timeManager->worldTime - playerInfo->lastScoreRestrictionTime >= SCORE_RESTRICTION_TIME) {
		playerInfo->scoreCanBeIncreased = true;
		return;
	}
}


void CheckHPIncreaseConditions(playerInfo* playerInfo) {
	if (playerInfo->userScore > playerInfo->lastHPIncrease + HP_INCREASE_SCORE_THRESHOLD && playerInfo->hp < MAX_HP) {
		playerInfo->hp++;
		playerInfo->lastHPIncrease += HP_INCREASE_SCORE_THRESHOLD;
	}
}


void PlayerShoot(playerInfo* playerInfo, weapons* weapon) {
	weapon->onBoard = true;
	weapon->weaponHit = false;
	weapon->positionStart[X] = playerInfo->playerPosition[X];
	weapon->positionStart[Y] = playerInfo->playerPosition[Y] - (playerInfo->weaponMode == LASER ? LASER_OFFSET : WEAPON_OFFSET);
	weapon->position[X] = playerInfo->playerPosition[X];
	weapon->position[Y] = playerInfo->playerPosition[Y] - (playerInfo->weaponMode == LASER ? LASER_OFFSET : WEAPON_OFFSET);
	weapon->type = playerInfo->weaponMode;
	switch (weapon->type)
	{
	case BULLET:
		weapon->velocity = velocity::bulletVelocity;
		weapon->range = range::bulletRange;
		weapon->damage = damage::bulletDamage;
		break;
	case ROCKET:
		weapon->velocity = velocity::rocketVelocity;
		weapon->range = range::rocketRange;
		weapon->damage = damage::rocketDamage;
		playerInfo->ammo--;
		break;
	case LASER:
		weapon->velocity = velocity::laserVelocity;
		weapon->range = range::laserRange;
		weapon->damage = damage::laserDamage;
		playerInfo->ammo--;
		break;
	}
	if (playerInfo->ammo == 0)
		playerInfo->weaponMode = BULLET;
}


void CalculateWeaponData(weapons* weapon, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager) {
	if (!weapon->onBoard)
		return;

	weapon->position[Y] -= weapon->velocity;

	if (weapon->positionStart[Y] - weapon->position[Y] > weapon->range)
		weapon->onBoard = false;

	if (weapon->weaponHit)
		return;

	if (abs(weapon->position[Y] - enemyInfo->enemyPosition[Y]) < VERTICAL_COLLISION_OFFSET &&
		abs(weapon->position[X] - enemyInfo->enemyPosition[X]) < HORIZONTAL_COLLISION_OFFSET &&
		!weapon->weaponHit) {
		weapon->weaponHit = true;
		enemyInfo->enemyHP -= weapon->damage;
		if (enemyInfo->enemyHP <= 0) {
			DestroyEnemy(enemyInfo, ENEMY_SHOOT);
			if (playerInfo->scoreCanBeIncreased)
				playerInfo->userScoreTemp += SHOOT_SCORE_INCREASE_RATE;
		}
	}

	if (abs(weapon->position[Y] - enemyInfo->enemy2Position[Y]) < VERTICAL_COLLISION_OFFSET &&
		abs(weapon->position[X] - enemyInfo->enemy2Position[X]) < HORIZONTAL_COLLISION_OFFSET &&
		!weapon->weaponHit) {
		weapon->weaponHit = true;
		enemyInfo->enemy2HP -= weapon->damage;
		if (enemyInfo->enemy2HP <= 0) {
			DestroyEnemy(enemyInfo, ENEMY_FORCE_OUT);
			if (playerInfo->scoreCanBeIncreased)
				playerInfo->userScoreTemp += SHOOT_SCORE_INCREASE_RATE;
		}
	}

	if (abs(weapon->position[Y] - npcInfo->npcPosition[Y]) < VERTICAL_COLLISION_OFFSET &&
		abs(weapon->position[X] - npcInfo->npcPosition[X]) < HORIZONTAL_COLLISION_OFFSET &&
		!weapon->weaponHit) {
		weapon->weaponHit = true;
		npcInfo->npcHP -= weapon->damage;
		if (npcInfo->npcHP <= 0) {
			DestroyNpc(npcInfo);
			FreezePointsGain(playerInfo, timeManager);
		}
	}
}


void HandlePowerUp(powerUp* powerUp, playerInfo* playerInfo) {

	if (playerInfo->weaponMode != BULLET)
		return;

	if (powerUp->onBoard) {
		powerUp->position[Y] += POWER_UP_POSITION_CHANGE_RATE;

		if (abs(playerInfo->playerPosition[Y] - powerUp->position[Y]) < VERTICAL_COLLISION_OFFSET &&
			abs(playerInfo->playerPosition[X] - powerUp->position[X]) < HORIZONTAL_COLLISION_OFFSET) {
			powerUp->onBoard = false;
			playerInfo->weaponMode = powerUp->type;
			playerInfo->ammo = powerUp->type == powerUpType::laserGun ? powerUpAmmo::laserGunAmmo : powerUpAmmo::rocketLauncherAmmo;
		}
		if (powerUp->position[Y] > SCREEN_BORDER_BOTTOM)
			powerUp->onBoard = false;

		return;
	}

	srand(time(NULL));
	int seed = rand() % POWER_UP_APPEARANCE_RAND;
	if (seed > POWER_UP_APPEARANCE_CONDITION)
		return;

	powerUp->onBoard = true;

	seed = rand() % POWER_UP_TYPE_RAND;
	if (seed > POWER_UP_TYPE_CONDITION)
		powerUp->type = powerUpType::laserGun;
	else
		powerUp->type = powerUpType::rocketLauncher;
	powerUp->position[Y] = 0;

	seed = rand() % LANE_CHOICE_RAND;
	switch (seed)
	{
	case 0:
		powerUp->position[X] = LANE_0_POSITION;
		break;
	case 1:
		powerUp->position[X] = LANE_1_POSITION;
		break;
	case 2:
		powerUp->position[X] = LANE_2_POSITION;
		break;
	case 3:
		powerUp->position[X] = LANE_3_POSITION;
		break;
	default:
		break;
	}
}