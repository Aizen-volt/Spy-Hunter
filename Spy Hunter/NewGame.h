#pragma once

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

void NewGameCreatePlayerData(playerInfo* playerInfo);

void NewGameCreateNPCData(npcInfo* npcInfo);

void NewGameCreateEnemyData(enemyInfo* enemyInfo);

void NewGameCreateWeaponData(weapons* weapon);

void NewGameCreateData(timeManagement* timeManager, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, weapons* weapon);

void NewGame(windowManagement* windowManager, roadBMPs* roadBMPs, carsBMPs* carsBMPs, weaponsBMPs* weaponsBMPs, weapons* weapon, color* colors, timeManagement* timeManager, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, powerUp* powerUp);