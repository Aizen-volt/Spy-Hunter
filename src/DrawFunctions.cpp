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

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};


void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};


void DrawMapBMPs(SDL_Surface* screen, roadBMPs* roadBMPs, double bmpsOffset) {
	DrawSurface(screen, roadBMPs->road, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	DrawSurface(screen, roadBMPs->roadLanes[LEFT], LEFT_LANE_POSITION, bmpsOffset);
	DrawSurface(screen, roadBMPs->roadLanes[RIGHT], RIGHT_LANE_POSITION, bmpsOffset);
	DrawSurface(screen, roadBMPs->roadTrees[LEFT], LEFT_TREES_POSITION, bmpsOffset);
	DrawSurface(screen, roadBMPs->roadTrees[RIGHT], RIGHT_TREES_POSITION, bmpsOffset);
}


void DrawLegends(SDL_Surface* screen, color* colors, SDL_Surface* charset, SDL_Surface* heart, double worldTime, int userScore, int hp, int ammo) {
	char text[128];
	

	DrawRectangle(screen, 0, 0, SCREEN_WIDTH, CONTROLS_LEGEND_HEIGHT, colors->white, colors->black);
	DrawRectangle(screen, FUNCTIONALITIES_POSITION_X, FUNCTIONALITIES_POSITION_Y, FUNCTIONALITIES_WIDTH, FUNCTIONALITIES_HEIGHT, colors->white, colors->black);

	sprintf(text, "Mateusz Kaszubowski 193050");
	DrawString(screen, TEXT_ALLIGN_CENTER, TITLE_POSITION_Y, text, charset);

	for (int i = 0; i < hp; i++) {
		DrawSurface(screen, heart, HEART_POSITION_X, HEART_POSITION_Y);
	}

	if (ammo == 0)
		sprintf(text, "TIME - %.1lfs  SCORE - %d AMMO - inf", worldTime, userScore);
	else
		sprintf(text, "TIME - %.1lfs  SCORE - %d AMMO - %d", worldTime, userScore, ammo);
	DrawString(screen, TEXT_ALLIGN_CENTER, LEGEND_1_POSITION_Y, text, charset);


	/*
	  ARROW_LEFT \032
	  ARROW_UP \030
	  ARROW_BOTTOM \031
	  ARROW_RIGHT \033
	  */
	sprintf(text, "\032\030\031\033 - move, esc - exit, n - new game, s - save game");
	DrawString(screen, TEXT_ALLIGN_CENTER, LEGEND_2_POSITION_Y, text, charset);

	sprintf(text, "l - load game, p - pause, space - shoot, f - finish game, t - score list");
	DrawString(screen, TEXT_ALLIGN_CENTER, LEGEND_3_POSITION_Y, text, charset);

	sprintf(text, "Functionalities");
	DrawString(screen, TEXT_ALLIGN_CENTER_FUNCTIONALITIES, FUNCTIONALITIES_TITLE_POSITION_Y, text, charset);

	sprintf(text, "a, b, c, d, e, f, g");
	DrawString(screen, TEXT_ALLIGN_CENTER_FUNCTIONALITIES, FUNCTIONALITIES_1_POSITION_Y, text, charset);
	sprintf(text, "i, j, k, l, m, n, o");
	DrawString(screen, TEXT_ALLIGN_CENTER_FUNCTIONALITIES, FUNCTIONALITIES_2_POSITION_Y, text, charset);

}


void DisplayPauseScreen(SDL_Surface* screen, color* colors, SDL_Surface* charset) {
	char text[128];

	DrawRectangle(screen, 0, POP_UP_POSITION_Y, SCREEN_WIDTH, POP_UP_HEIGHT, colors->white, colors->black);

	sprintf(text, "Game paused! Press P to continue...");
	DrawString(screen, TEXT_ALLIGN_CENTER, POP_UP_TEXT_POSITION_Y, text, charset);
}


void DisplaySaveGameWindow(windowManagement* windowManager, color* colors, char* fileName) {
	char text[128];


	DrawRectangle(windowManager->screen, 0, POP_UP_POSITION_Y, SCREEN_WIDTH, POP_UP_HEIGHT, colors->white, colors->black);
	sprintf(text, "File %s saved correctly", fileName);
	DrawString(windowManager->screen, windowManager->TEXT_ALLIGN_CENTER, POP_UP_TEXT_POSITION_Y, text, windowManager->charset);

	UpdateScreenOutput(windowManager);

	SDL_Delay(20 * INPUT_DELAY);
	SDL_PumpEvents();
}


void DisplayLoadNoFilesFound(windowManagement* windowManager, color* colors) {
	char text[128];


	DrawRectangle(windowManager->screen, 0, POP_UP_POSITION_Y, SCREEN_WIDTH, POP_UP_HEIGHT, colors->white, colors->black);
	sprintf(text, "No savefiles found!");
	DrawString(windowManager->screen, windowManager->TEXT_ALLIGN_CENTER, POP_UP_TEXT_POSITION_Y, text, windowManager->charset);

	UpdateScreenOutput(windowManager);

	SDL_Delay(20 * INPUT_DELAY);
	SDL_PumpEvents();
}


void DisplayFilesToLoad(windowManagement* windowManager, color* colors, int filesFound, char fileNames[][128], int cursor) {
	char text[128];

	DrawRectangle(windowManager->screen, 0, LOAD_FILE_POSITION_Y, SCREEN_WIDTH, LOAD_FILE_HEIGHT, colors->white, colors->black);
	sprintf(text, "Press L to select save:");
	DrawString(windowManager->screen, windowManager->TEXT_ALLIGN_CENTER, LOAD_FILE_TITLE_POSITION_Y, text, windowManager->charset);

	DrawRectangle(windowManager->screen, LOAD_FILE_OUTLINE_POSITION_X, LOAD_FILE_OUTLINE_POSITION_Y, LOAD_FILE_OUTLINE_WIDTH, LOAD_FILE_OUTLINE_HEIGHT, colors->black, colors->gray);

	for (int i = 0; i < filesFound; i++) {
		sprintf(text, "%d) %s", i + 1, fileNames[i]);
		DrawString(windowManager->screen, windowManager->TEXT_ALLIGN_CENTER, LOAD_FILE_FILE_POSITION_Y, text, windowManager->charset);
	}

	UpdateScreenOutput(windowManager);
}


void DisplayFileError(windowManagement* windowManager, color* colors) {
	char text[128];

	DrawRectangle(windowManager->screen, 0, POP_UP_POSITION_Y, SCREEN_WIDTH, POP_UP_HEIGHT, colors->white, colors->black);
	sprintf(text, "An error occured while opening the file!");
	DrawString(windowManager->screen, windowManager->TEXT_ALLIGN_CENTER, POP_UP_TEXT_POSITION_Y, text, windowManager->charset);

	UpdateScreenOutput(windowManager);

	SDL_Delay(20 * INPUT_DELAY);
	SDL_PumpEvents();
}


void DrawCars(SDL_Surface* screen, carsBMPs* carsBMPs, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo) {
	DrawSurface(screen, carsBMPs->playerBMP, playerInfo->playerPosition[X], playerInfo->playerPosition[Y]);
	DrawSurface(screen, carsBMPs->enemyBMP, enemyInfo->enemyPosition[X], enemyInfo->enemyPosition[Y]);
	DrawSurface(screen, carsBMPs->enemy2BMP, enemyInfo->enemy2Position[X], enemyInfo->enemy2Position[Y]);

	if (npcInfo->startLane > 1)
		DrawSurface(screen, carsBMPs->npcsBMP, npcInfo->npcPosition[X], npcInfo->npcPosition[Y]);
	else
		DrawSurface(screen, carsBMPs->npcs2BMP, npcInfo->npcPosition[X], npcInfo->npcPosition[Y]);
}


void DisplayGameFinishedScreen(windowManagement* windowManager, color* colors, double worldTime, int userScore) {
	char text[128];

	DrawRectangle(windowManager->screen, 0, POP_UP_POSITION_Y, SCREEN_WIDTH, GAME_FINISHED_HEIGHT, colors->white, colors->black);

	sprintf(text, "Game finished with time %.1lfs and score %d", worldTime, userScore);
	DrawString(windowManager->screen, windowManager->TEXT_ALLIGN_CENTER, GAME_FINISHED_TEXT_1_POSITION_Y, text, windowManager->charset);

	sprintf(text, "Add score to the score list? [Y/N]");
	DrawString(windowManager->screen, windowManager->TEXT_ALLIGN_CENTER, GAME_FINISHED_TEXT_2_POSITION_Y, text, windowManager->charset);
}


void DisplayScoreList(windowManagement* windowManager, color* colors, timeManagement* timeManager) {
	char text[128];

	int numberOfScores = CountScores(windowManager, colors);
	double* times = new double[numberOfScores];
	int* scores = new int[numberOfScores];
	int pageNumber = 0;

	GetScoresFromFile(numberOfScores, times, scores);

	while (true) {
		timeManager->tick1WithoutPause = SDL_GetTicks() - timeManager->tick2;

		DrawRectangle(windowManager->screen, 0, SCORE_POSITION_Y, SCREEN_WIDTH, SCORE_HEIGHT, colors->white, colors->black);
		sprintf(text, "SCORES (p - sort by points, t - sort by time, \031 - page down, \030 - page up)");
		DrawString(windowManager->screen, windowManager->TEXT_ALLIGN_CENTER, SCORE_TITLE_POSITION_Y, text, windowManager->charset);

		for (int i = 0; i < SCORES_ON_PAGE; i++) {
			if (pageNumber * SCORES_ON_PAGE + i >= numberOfScores)
				break;
			sprintf(text, "%d) %.1lf %d", pageNumber * SCORES_ON_PAGE + i + 1, times[pageNumber * SCORES_ON_PAGE + i], scores[pageNumber * SCORES_ON_PAGE + i]);
			DrawString(windowManager->screen, SCORE_SCORE_POSITION_X, SCORE_SCORE_POSITION_Y, text, windowManager->charset);
		}

		SDL_Delay(INPUT_DELAY);
		const Uint8* state = SDL_GetKeyboardState(NULL);

		if (state[SDL_SCANCODE_UP] && pageNumber > 0)
			pageNumber--;
		if (state[SDL_SCANCODE_DOWN] && pageNumber < numberOfScores / SCORES_ON_PAGE)
			pageNumber++;
		if (state[SDL_SCANCODE_P])
			SortScoresByPoints(numberOfScores, times, scores);
		if (state[SDL_SCANCODE_T])
			SortScoresByTime(numberOfScores, times, scores);
		if (state[SDL_SCANCODE_BACKSPACE])
			break;

		SDL_PumpEvents();
		printf("%d", pageNumber);
		UpdateScreenOutput(windowManager);
	}

	delete[] times;
	delete[] scores;
}


void SortScoresByPoints(int numberOfScores, double times[], int points[]) {
	double tempTimes;
	int tempPoints;

	for (int i = 0; i < numberOfScores; i++) {
		for (int j = i; j < numberOfScores - 1; j++) {
			if (points[j] > points[j + 1]) {
				tempPoints = points[j];
				points[j] = points[j + 1];
				points[j + 1] = tempPoints;

				tempTimes = times[j];
				times[j] = times[j + 1];
				times[j + 1] = tempTimes;
			}
		}
	}
}


void SortScoresByTime(int numberOfScores, double times[], int points[]) {
	double tempTimes;
	int tempPoints;

	for (int i = 0; i < numberOfScores; i++) {
		for (int j = i; j < numberOfScores - 1; j++) {
			if (times[j] > times[j + 1]) {
				tempPoints = points[j];
				points[j] = points[j + 1];
				points[j + 1] = tempPoints;

				tempTimes = times[j];
				times[j] = times[j + 1];
				times[j + 1] = tempTimes;
			}
		}
	}
}


void DrawWeapons(SDL_Surface* screen, weaponsBMPs* weaponsBMPs, weapons* weapon) {
	if (!weapon->weaponHit) {
		switch (weapon->type)
		{
		case BULLET:
			DrawSurface(screen, weaponsBMPs->bulletBMP, weapon->position[X], weapon->position[Y]);
			break;
		case ROCKET:
			DrawSurface(screen, weaponsBMPs->rocketBMP, weapon->position[X], weapon->position[Y]);
			break;
		case LASER:
			DrawSurface(screen, weaponsBMPs->laserBMP, weapon->position[X], weapon->position[Y]);
			break;
		default:
			break;
		}
	}
}


void DrawPowerUp(SDL_Surface* screen, weaponsBMPs* weaponsBMPs, powerUp* powerUp) {
	if (powerUp->onBoard) {
		powerUp->type == powerUpType::laserGun ?
			DrawSurface(screen, weaponsBMPs->laserGunBMP, powerUp->position[X], powerUp->position[Y])
			:
			DrawSurface(screen, weaponsBMPs->rocketLauncherBMP, powerUp->position[X], powerUp->position[Y]);
	}
}