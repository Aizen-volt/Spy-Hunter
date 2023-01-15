#pragma once

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

void InitializeSDL();

void CreateWindowAndRenderer(windowManagement* windowManager);

void ERRORbmpLoadExit(windowManagement* windowManager, const char* bmpName);

void PrepareGame(windowManagement* windowManager);

void LoadRoadBMPs(windowManagement* windowManager, roadBMPs* roadBMPs);

void LoadRGBMaps(SDL_Surface* screen, color* colors);

void UpdateScreenOutput(windowManagement* windowManager);

void FreeSurfaces(windowManagement* windowManager);

void EndProgram(windowManagement* windowManager);

void LoadCarsBMPs(windowManagement* windowManager, carsBMPs* carsBMPs);

void LoadWeaponsBMPs(windowManagement* windowManager, weaponsBMPs* weaponsBMPs);