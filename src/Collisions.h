#pragma once

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

void EnemyNPCCollision(enemyInfo* enemyInfo, npcInfo* npcInfo);

void Enemy2NPCCollision(enemyInfo* enemyInfo, npcInfo* npcInfo);

void PlayerEnemy2Collision(windowManagement* windowManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager);

void PlayerEnemyCollision(playerInfo* playerInfo, enemyInfo* enemyInfo);

void PlayerEnemyCollisionPlayerPush(playerInfo* playerInfo, enemyInfo* enemyInfo);

void PlayerEnemy2CollisionPlayerPush(playerInfo* playerInfo, enemyInfo* enemyInfo);

void EnemyEnemy2Collision(enemyInfo* enemyInfo);

void CheckCollisionsOnMove(windowManagement* windowManager, color* colors, playerInfo* playerInfo, enemyInfo* enemyInfo, npcInfo* npcInfo, timeManagement* timeManager);

void CheckCollisionsEnemies(windowManagement* windowManager, color* colors, playerInfo* playerInfo, enemyInfo* enemyInfo, npcInfo* npcInfo, timeManagement* timeManager);

void PlayerNPCCollision(windowManagement* windowManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager);

void CheckPlayerBeyondMap(windowManagement* windowManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager);

void CheckEnemyBeyondMap(enemyInfo* enemyInfo, playerInfo* playerInfo);

void CheckEnemy2BeyondMap(enemyInfo* enemyInfo, playerInfo* playerInfo);

void CheckNPCBeyondMap(npcInfo* npcInfo, playerInfo* playerInfo, timeManagement* timeManager);