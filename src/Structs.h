#pragma once

extern "C" {
#include"../SDL2-2.0.10/include/SDL.h"
#include"../SDL2-2.0.10/include/SDL_main.h"
}

typedef struct {
	int tick1;
	int tick2;
	int fps;
	int frames;
	double fpsTimer;
	double deltaTime;
	double worldTime;
	double distance;
	double gameSpeed;
	int tick1WithoutPause;
} timeManagement;


typedef struct {
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Surface* eti;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
} windowManagement;


typedef struct {
	SDL_Surface* road;
	SDL_Surface* roadLanes[2];
	SDL_Surface* roadTrees[2];
	SDL_Surface* heart;
} roadBMPs;


typedef struct {
	double userScoreTemp;
	int userScore;
	bool scoreCanBeIncreased;
	double lastScoreRestrictionTime;
	int playerPosition[2];
	int hp;
	char weaponMode;
	int ammo;
	int lastHPIncrease;
} playerInfo;


enum velocity {
	bulletVelocity = 30,
	rocketVelocity = 10,
	laserVelocity = 40
};


enum range {
	bulletRange = 500,
	rocketRange = 1000,
	laserRange = 400
};


enum damage {
	bulletDamage = 2,
	rocketDamage = 100,
	laserDamage = 2
};


typedef struct {
	bool onBoard;
	char type;
	int position[2];
	int positionStart[2];
	int velocity;
	int range;
	int damage;
	bool weaponHit;
} weapons;


typedef struct {
	bool npcPresentOnBoard;
	int npcPosition[2];
	int startLane;
	int npcHP;
} npcInfo;


typedef struct {
	bool enemyPresentOnBoard[2];
	int enemyPosition[2];
	int enemy2Position[2];
	int enemyStartLane;
	int enemy2StartLane;
	int enemyHP;
	int enemy2HP;
} enemyInfo;


typedef struct {
	SDL_Surface* playerBMP;
	SDL_Surface* npcsBMP;
	SDL_Surface* npcs2BMP;
	SDL_Surface* enemyBMP;
	SDL_Surface* enemy2BMP;
} carsBMPs;


typedef struct {
	SDL_Surface* rocketBMP;
	SDL_Surface* laserBMP;
	SDL_Surface* bulletBMP;
	SDL_Surface* rocketLauncherBMP;
	SDL_Surface* laserGunBMP;
} weaponsBMPs;


enum powerUpType {
	rocketLauncher = ROCKET,
	laserGun = LASER
};


enum powerUpAmmo {
	rocketLauncherAmmo = 2,
	laserGunAmmo = 40
};


typedef struct {
	bool onBoard;
	int type;
	int position[2];
}powerUp;


typedef struct {
	int black;
	int green;
	int white;
	int blue;
	int gray;
} color;