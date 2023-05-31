#pragma once

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

void RandomizeNPCAppearance(npcInfo* npcInfo);

void MoveNPC(npcInfo* npcInfo, timeManagement* timeManager);

void HandleNPC(npcInfo* npcInfo, timeManagement* timeManager);

void RandomizeEnemyAppearance(enemyInfo* enemyInfo, npcInfo* npcInfo, int enemyType);

void MoveEnemies(windowManagement* windowManager, color* colors, playerInfo* playerInfo, enemyInfo* enemyInfo, npcInfo* npcInfo, double distance, timeManagement* timeManager);

void HandleEnemy(windowManagement* windowManager, color* colors, playerInfo* playerInfo, enemyInfo* enemyInfo, npcInfo* npcInfo, double distance, timeManagement* timeManager);