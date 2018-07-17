#pragma once
#include "def.h"

// BRM Values
int x;
int y;
int v_x;
int v_y;
char is_left;
int hp;
int Ehp;

// Enemy Values
int Ex;
int Ey;
int Ev_y;
int Edirection;

// BRM Graphic Pointer
const char (*p_br)[P_WIDTH];

#define BULLET_ARRAY_SIZE 4
bullet bullets[BULLET_ARRAY_SIZE];
int bullet_next;

#define WEAPON_ARRAY_SIZE 16
weapon weapons[WEAPON_ARRAY_SIZE];
int weapon_next;

int enemy_num;

int counter;
int last_shot;
int last_damage;
int last_Eshot;
int Enext_routine_time;

char result;
char battle_start;

int rand_seed;
