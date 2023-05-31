#pragma once

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

int CountTXTFiles(windowManagement* windowManager, color* colors);

void SaveFileNamesToArray(windowManagement* windowManager, color* colors, char fileNames[][128]);

void LoadDataFromFile(windowManagement* windowManager, color* colors, timeManagement* timeManager, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, char* fileName, weapons* weapon, powerUp* powerUp);

void LoadGame(windowManagement* windowManager, timeManagement* timeManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, weapons* weapon, powerUp* powerUp);

void SaveGame(windowManagement* windowManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager, weapons* weapon, powerUp* powerUp);

void AddScoreToList(int userScore, double worldTime);

int CountScores(windowManagement* windowManager, color* colors);

void GetScoresFromFile(int numberOfScores, double times[], int scores[]);