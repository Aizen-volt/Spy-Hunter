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

int CountTXTFiles(windowManagement* windowManager, color* colors) {
	int filesFound = 0;
	struct _finddata_t dirFile;
	intptr_t file;

	if ((file = _findfirst("*.dat", &dirFile)) == -1L) {
		DisplayLoadNoFilesFound(windowManager, colors);
		return -1;
	}
	else {
		do {
			filesFound++;
		} while (_findnext(file, &dirFile) == 0);
	}
	_findclose(file);
	return filesFound;
}


void SaveFileNamesToArray(windowManagement* windowManager, color* colors, char fileNames[][128]) {
	struct _finddata_t dirFile;
	intptr_t file;
	int i = 0;

	if ((file = _findfirst("*.dat", &dirFile)) == -1L) {
		DisplayLoadNoFilesFound(windowManager, colors);
		return;
	}
	else {
		do {
			int j = 0;
			while (dirFile.name[j] != '\0') {
				fileNames[i][j] = dirFile.name[j];
				j++;
			}
			fileNames[i][j] = '\0';
			i++;
		} while (_findnext(file, &dirFile) == 0);
	}
	_findclose(file);
}


void LoadDataFromFile(windowManagement* windowManager, color* colors, timeManagement* timeManager, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, char* fileName, weapons* weapon, powerUp* powerUp) {
	FILE* file;
	file = fopen(fileName, "rb");
	if (file == NULL) {
		DisplayFileError(windowManager, colors);
		return;
	}

	fread(playerInfo, sizeof(*playerInfo), 1, file);
	fread(npcInfo, sizeof(*npcInfo), 1, file);
	fread(enemyInfo, sizeof(*enemyInfo), 1, file);
	fread(timeManager, sizeof(*timeManager), 1, file);
	fread(weapon, sizeof(*weapon), 1, file);
	fread(powerUp, sizeof(*powerUp), 1, file);

	fclose(file);
}


void LoadGame(windowManagement* windowManager, timeManagement* timeManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, weapons* weapon, powerUp* powerUp) {
	int cursor = 0;
	int filesFound = CountTXTFiles(windowManager, colors);
	if (filesFound == -1)
		return;

	auto fileNames = new char[filesFound][128];
	SaveFileNamesToArray(windowManager, colors, fileNames);

	while (true) {
		DisplayFilesToLoad(windowManager, colors, filesFound, fileNames, cursor);
		SDL_Delay(INPUT_DELAY);
		const Uint8* state = SDL_GetKeyboardState(NULL);
		SDL_PumpEvents();
		if (state[SDL_SCANCODE_UP] && cursor > 0)
			cursor--;
		if (state[SDL_SCANCODE_DOWN] && cursor < filesFound - 1)
			cursor++;
		if (state[SDL_SCANCODE_RETURN]) {
			LoadDataFromFile(windowManager, colors, timeManager, playerInfo, npcInfo, enemyInfo, fileNames[cursor], weapon, powerUp);
			SDL_Delay(1000);
			SDL_PumpEvents();
			break;
		}
		if (state[SDL_SCANCODE_BACKSPACE])
			break;
	}
	delete fileNames;

	timeManager->tick1WithoutPause = SDL_GetTicks() - timeManager->tick2;
}


void SaveGame(windowManagement* windowManager, color* colors, playerInfo* playerInfo, npcInfo* npcInfo, enemyInfo* enemyInfo, timeManagement* timeManager, weapons* weapon, powerUp* powerUp) {
	time_t rawTime;
	struct tm* timeInfo;
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	char fileName[128];

	strftime(fileName, sizeof(fileName), "%d-%m-%Y-%H-%M-%S", timeInfo);

	int i = 0;
	while (fileName[i] != '\0') {
		i++;
	}

	fileName[i] = '.';
	fileName[i + 1] = 'd';
	fileName[i + 2] = 'a';
	fileName[i + 3] = 't';
	fileName[i + 4] = '\0';

	FILE* file;
	file = fopen(fileName, "wb");
	if (file == NULL) {
		DisplayFileError(windowManager, colors);
		return;
	}

	fwrite(playerInfo, sizeof(*playerInfo), 1, file);
	fwrite(npcInfo, sizeof(*npcInfo), 1, file);
	fwrite(enemyInfo, sizeof(*enemyInfo), 1, file);
	fwrite(timeManager, sizeof(*timeManager), 1, file);
	fwrite(weapon, sizeof(*weapon), 1, file);
	fwrite(powerUp, sizeof(*powerUp), 1, file);


	DisplaySaveGameWindow(windowManager, colors, fileName);
	fclose(file);

	timeManager->tick1WithoutPause += SDL_GetTicks() - timeManager->tick2;
}


void AddScoreToList(int userScore, double worldTime) {
	FILE* file;
	file = fopen("score.txt", "a");
	fprintf(file, "%.1lf %d\n", worldTime, userScore);
}


int CountScores(windowManagement* windowManager, color* colors) {
	FILE* file;
	file = fopen("score.txt", "r");

	if (file == NULL)
		DisplayFileError(windowManager, colors);

	int numberOfLines = 0;

	for (char c = getc(file); c != EOF; c = getc(file))
		if (c == '\n')
			numberOfLines++;
	printf("%d", numberOfLines);
	return numberOfLines;
}


void GetScoresFromFile(int numberOfScores, double times[], int scores[]) {
	FILE* file;
	file = fopen("score.txt", "r");

	for (int i = 0; i < numberOfScores; i++)
		fscanf(file, "%lf %d\n", &times[i], &scores[i]);
}