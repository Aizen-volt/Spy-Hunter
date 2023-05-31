#pragma once

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);

void DrawMapBMPs(SDL_Surface* screen, roadBMPs* roadBMPs, double bmpsOffset);

void DrawLegends(SDL_Surface* screen, color* colors, SDL_Surface* charset, SDL_Surface* heart, double worldTime, int userScore, int hp, int ammo);

void DisplayPauseScreen(SDL_Surface* screen, color* colors, SDL_Surface* charset);

void DisplaySaveGameWindow(windowManagement* windowManager, color* colors, char* fileName);

void DisplayLoadNoFilesFound(windowManagement* windowManager, color* colors);

void DisplayFilesToLoad(windowManagement* windowManager, color* colors, int filesFound, char fileNames[][128], int cursor);

void DisplayFileError(windowManagement* windowManager, color* colors);

void DrawCars(SDL_Surface* screen, carsBMPs* carsBMPs, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo);

void DisplayGameFinishedScreen(windowManagement* windowManager, color* colors, double worldTime, int userScore);

void DisplayScoreList(windowManagement* windowManager, color* colors, timeManagement* timeManager);

void SortScoresByPoints(int numberOfScores, double times[], int points[]);

void SortScoresByTime(int numberOfScores, double times[], int points[]);

void DrawWeapons(SDL_Surface* screen, weaponsBMPs* weaponsBMPs, weapons* weapon);

void DrawPowerUp(SDL_Surface* screen, weaponsBMPs* weaponsBMPs, powerUp* powerUp);