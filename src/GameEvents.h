#pragma once

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

void HoldGameFrame(timeManagement* timeManager, windowManagement* windowManager, color* colors, roadBMPs* roadBMPs, carsBMPs* carsBMPs, weaponsBMPs* weaponsBMPs, weapons* weapon, double bmpsOffset, playerInfo* playerInfo, enemyInfo* enemyInfo, npcInfo* npcInfo, powerUp* powerUp);

void PauseGame(windowManagement* windowManager, color* colors, timeManagement* timeManager);

void ResetMap(playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo);

void FinishGame(windowManagement* windowManager, color* colors, timeManagement* timeManager, playerInfo* playerInfo);

void DestroyPlayer(windowManagement* windowManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager);

void DestroyEnemy(enemyInfo* enemyInfo, int enemyType);

void DestroyNpc(npcInfo* npcInfo);

void FreezePointsGain(playerInfo* playerInfo, timeManagement* timeManager);

void PlayerMove(playerInfo* playerInfo, int x, int y);

void CheckScoreIncreaseConditions(playerInfo* playerInfo, timeManagement* timeManager);

void CheckHPIncreaseConditions(playerInfo* playerInfo);

void PlayerShoot(playerInfo* playerInfo, weapons* weapon);

void CalculateWeaponData(weapons* weapon, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager);

void HandlePowerUp(powerUp* powerUp, playerInfo* playerInfo);