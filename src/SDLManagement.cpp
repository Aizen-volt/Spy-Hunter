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

void InitializeSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(1);
	}
}


void CreateWindowAndRenderer(windowManagement* windowManager) {
	int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &windowManager->window, &windowManager->renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		exit(1);
	};
}


void ERRORbmpLoadExit(windowManagement* windowManager, const char* bmpName) {
	printf("SDL_LoadBMP(%s) error: %s\n", bmpName, SDL_GetError());
	SDL_FreeSurface(windowManager->charset);
	SDL_FreeSurface(windowManager->screen);
	SDL_DestroyTexture(windowManager->scrtex);
	SDL_DestroyWindow(windowManager->window);
	SDL_DestroyRenderer(windowManager->renderer);
	SDL_Quit();
	exit(1);
}


void PrepareGame(windowManagement* windowManager) {
	InitializeSDL();
	CreateWindowAndRenderer(windowManager);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(windowManager->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(windowManager->renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(windowManager->window, "Mateusz Kaszubowski 193050");

	windowManager->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	windowManager->scrtex = SDL_CreateTexture(windowManager->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	windowManager->charset = SDL_LoadBMP("../cs8x8.bmp");
	if (windowManager->charset == NULL) {
		ERRORbmpLoadExit(windowManager, "cs8x8.bmp");
	};
	SDL_SetColorKey(windowManager->charset, true, 0x000000);
}


void LoadRoadBMPs(windowManagement* windowManager, roadBMPs* roadBMPs) {
	roadBMPs->road = SDL_LoadBMP("../road.bmp");
	if (roadBMPs->road == NULL) {
		ERRORbmpLoadExit(windowManager, "road.bmp");
	};

	for (int i = 0; i < 2; i++) {
		roadBMPs->roadLanes[i] = SDL_LoadBMP("../lane.bmp");
		if (roadBMPs->roadLanes[i] == NULL) {
			ERRORbmpLoadExit(windowManager, "lane.bmp");
		};
	}

	for (int i = 0; i < 2; i++) {
		roadBMPs->roadTrees[i] = SDL_LoadBMP("../trees.bmp");
		if (roadBMPs->roadTrees[i] == NULL) {
			ERRORbmpLoadExit(windowManager, "trees.bmp");
		};
	}

	roadBMPs->heart = SDL_LoadBMP("../heart.bmp");
	if (roadBMPs->heart == NULL) {
		ERRORbmpLoadExit(windowManager, "heart.bmp");
	};
}


void LoadRGBMaps(SDL_Surface* screen, color* colors) {
	colors->black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	colors->green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	colors->white = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	colors->blue = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	colors->gray = SDL_MapRGB(screen->format, 0xAC, 0xAA, 0xA6);
}


void UpdateScreenOutput(windowManagement* windowManager) {
	SDL_UpdateTexture(windowManager->scrtex, NULL, windowManager->screen->pixels, windowManager->screen->pitch);
	SDL_RenderClear(windowManager->renderer);
	SDL_RenderCopy(windowManager->renderer, windowManager->scrtex, NULL, NULL);
	SDL_RenderPresent(windowManager->renderer);
}


void FreeSurfaces(windowManagement* windowManager) {
	SDL_FreeSurface(windowManager->charset);
	SDL_FreeSurface(windowManager->screen);
	SDL_DestroyTexture(windowManager->scrtex);
	SDL_DestroyRenderer(windowManager->renderer);
	SDL_DestroyWindow(windowManager->window);
}


void EndProgram(windowManagement* windowManager) {
	FreeSurfaces(windowManager);
	SDL_Quit();
	exit(0);
}


void LoadCarsBMPs(windowManagement* windowManager, carsBMPs* carsBMPs) {
	carsBMPs->playerBMP = SDL_LoadBMP("../player.bmp");
	if (carsBMPs->playerBMP == NULL) {
		ERRORbmpLoadExit(windowManager, "player.bmp");
	};

	carsBMPs->npcsBMP = SDL_LoadBMP("../npc.bmp");
	if (carsBMPs->npcsBMP == NULL) {
		ERRORbmpLoadExit(windowManager, "npc.bmp");
	};

	carsBMPs->npcs2BMP = SDL_LoadBMP("../npc2.bmp");
	if (carsBMPs->npcsBMP == NULL) {
		ERRORbmpLoadExit(windowManager, "npc2.bmp");
	};

	carsBMPs->enemyBMP = SDL_LoadBMP("../enemy.bmp");
	if (carsBMPs->enemyBMP == NULL) {
		ERRORbmpLoadExit(windowManager, "enemy.bmp");
	};

	carsBMPs->enemy2BMP = SDL_LoadBMP("../enemy2.bmp");
	if (carsBMPs->enemy2BMP == NULL) {
		ERRORbmpLoadExit(windowManager, "enemy2.bmp");
	};
}


void LoadWeaponsBMPs(windowManagement* windowManager, weaponsBMPs* weaponsBMPs) {
	weaponsBMPs->bulletBMP = SDL_LoadBMP("../bullet.bmp");
	if (weaponsBMPs->bulletBMP == NULL) {
		ERRORbmpLoadExit(windowManager, "bullet.bmp");
	};

	weaponsBMPs->rocketBMP = SDL_LoadBMP("../rocket.bmp");
	if (weaponsBMPs->rocketBMP == NULL) {
		ERRORbmpLoadExit(windowManager, "rocket.bmp");
	};

	weaponsBMPs->laserBMP = SDL_LoadBMP("../laser.bmp");
	if (weaponsBMPs->laserBMP == NULL) {
		ERRORbmpLoadExit(windowManager, "laser.bmp");
	};

	weaponsBMPs->rocketLauncherBMP = SDL_LoadBMP("../rocketlauncher.bmp");
	if (weaponsBMPs->rocketLauncherBMP == NULL) {
		ERRORbmpLoadExit(windowManager, "rocketlauncher.bmp");
	};

	weaponsBMPs->laserGunBMP = SDL_LoadBMP("../lasergun.bmp");
	if (weaponsBMPs->laserGunBMP == NULL) {
		ERRORbmpLoadExit(windowManager, "lasergun.bmp");
	};
}