#pragma once

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

void DispatchInputFunctions(windowManagement* windowManager, timeManagement* timeManager, playerInfo* playerInfo, color* colors, enemyInfo* enemyInfo, npcInfo* npcInfo, weapons* weapon, powerUp* powerUp);