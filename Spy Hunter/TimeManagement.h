#pragma once

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

void InitializeTimeData(timeManagement* timeManager);

void CalculateTimeData(timeManagement* timeManager);