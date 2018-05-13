#pragma once
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>#include <time.h>
#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
enum ship_type
{
	Basic,
	Esquiva,
	outra
};

enum powerup_type
{
	Escudo,
	Gelo,
	Bateria,
	Mais,
	Vida,
	Acool
};

typedef struct {
	int x, y;
} position;

typedef struct {
	position pos;
	int dimx, dimy;
} ship;

typedef struct {
	int id;
	ship s;
	int lifes;
	int tecla; // 8 6 2 4
} player;

typedef struct {
	int width, height;
	int nShip_Enemy, nPowerups;
	int njogadores, nShip_defender;
	ship_invade shipI[50];
}map_settings;

typedef struct {
	position pos;
	int vx, vy;
	bool view;
}shot;

typedef struct {
	position pos;
	int vx, vy;
	bool view;
}bomb;


typedef struct {
	int dim, hp;
	int width, height;
	position pos;
	enum ship_type type;
}ship_invade;

typedef struct {
	int vx, vy;
	int dim;
	int raridade, duration;
	position pos;
	enum powerup_type type;
	bool view;
}powerups;

typedef struct {
	int dificuldade;
	bool play;
	int nMaxPlayers;
	int nNavesInimigas, nPowerUps, duracao, prob, nVidasI;
}game;