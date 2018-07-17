#pragma once

typedef unsigned long long int uint64;

// Display
#define D_WIDTH  128
#define D_HEIGHT 128
volatile uint64 *e_vram = (uint64*)0x900000;
void *DISPLAY[D_HEIGHT * D_WIDTH];

// Graphic Size
#define P_WIDTH  32
#define P_HEIGHT 32
#define B_WIDTH  8
#define B_HEIGHT 8
#define W_WIDTH  16
#define W_HEIGHT 16
#define T_WIDTH  8
#define T_HEIGHT 8
#define C_WIDTH  8
#define C_HEIGHT 16

volatile int  *e_time = (int *)0x80010c;
volatile char *e_gp1  = (char*)0x8001f0;
volatile char *e_gp2  = (char*)0x8001f1;
volatile char *e_gin  = (char*)0x8001ff;

volatile char *e_sw1  = (char*)0x8001fc;
volatile char *e_sw2  = (char*)0x8001fd;
volatile char *e_sw3  = (char*)0x8001fe;

// Inverted Button Status
#define L_PUSHED !*e_sw1
#define C_PUSHED !*e_sw2
#define R_PUSHED !*e_sw3

// Colors
const char BLACK   = 0;
const char RED     = 1;
const char GREEN   = 2;
const char YELLOW  = 3;
const char BLUE    = 4;
const char MAGENTA = 5;
const char CYAN    = 6;
const char WHITE   = 7;
const char NOT_USED_COLOR = 8;
const char TRANSPARENT    = 9;

/**********************************************************************/
// Stage Values
#define   wall_thickness 8
const int y_ceil         =          + wall_thickness;
const int y_ceil_b       =          + wall_thickness;
const int y_ceil_w       =          + wall_thickness;
const int y_floor        = D_HEIGHT - wall_thickness - P_HEIGHT + 1;
const int y_floor_b      = D_HEIGHT - wall_thickness - B_HEIGHT + 1;
const int y_floor_w      = D_HEIGHT - wall_thickness - W_HEIGHT + 1;
const int x_left_wall    =          + wall_thickness;
const int x_left_wall_b  =          + wall_thickness;
const int x_left_wall_w  =          + wall_thickness;
const int x_right_wall   = D_WIDTH  - wall_thickness - P_WIDTH  + 1;
const int x_right_wall_b = D_WIDTH  - wall_thickness - B_WIDTH  + 1;
const int x_right_wall_w = D_WIDTH  - wall_thickness - W_WIDTH  + 1;

/**********************************************************************/
// Bullet
typedef struct {
	int x;
	int y;
	int direction;
	char enable;
} bullet;

// Weapon
typedef struct {
	const char (*p)[W_WIDTH];
	int x;
	int y;
	int v_x;
	int v_y;
	int a_y;
	int damage;
	char invert_enable;
	char enable;
} weapon;

/**********************************************************************/
const int bullet_v_x = 8;
const int bullet_x_difference = 8;
const int bullet_y_difference_stop = 15;
const int shoot_interval   = 8;
const int damage_disable_1 = 8;
const int damage_disable_2 = 64;

const int blink_length_damage = 3;
const int blink_length_dead   = 2;
const int move_frame_length   = 4;

// Motion Values
const int v_x_0       = 4;
const int v_y_0_jump  = -12;
const int g           = 1;
const int knockback_v_x   = 4;
const int knockback_v_y   = 8;
const int knockback_decay = 1;

const int hp_bar_width = 8;
const int hp_bar_num   = 24;
const int hp_max       = 96;

/**********************************************************************/
// Enemies Definition
#define NUM_ENEMIES 4

#define UPPEMAN   0
#define CATMAN    1
#define SUITMAN   2
#define PHOTONMAN 3

#define NAME_LENGTH 9
const char enemy_name[NUM_ENEMIES][NAME_LENGTH] = {
	{ ' ', 'U', 'P', 'P', 'E', 'M', 'A', 'N', ' ' },
	{ ' ', 'C', 'A', 'T', 'M', 'A', 'N', ' ', ' ' },
	{ ' ', 'S', 'U', 'I', 'T', 'M', 'A', 'N', ' ' },
	{ 'P', 'H', 'O', 'T', 'O', 'N', 'M', 'A', 'N' }
};

#define NUM_ALPHABETS 26

#define STOP   0
#define JUMP   1
#define ATTACK 2

const int Ehp_max[NUM_ENEMIES]         = {  20,  20,  20,  20 };
const int Ewait_interval               = 64;
const int Ejump_interval[NUM_ENEMIES]  = {  64,  16,  48,  32 };
const int Eshoot_cooldown[NUM_ENEMIES] = {  24,   8,  16,  20 };   
const int Eshoot_pose_length           = 8;
const int weapon_y_difference_stop[NUM_ENEMIES]
                                       = {   6,  12,  12,  12 };
const int Ev_x_0[NUM_ENEMIES]          = {   2,   3,   4,   3 };
const int Ev_y_0_jump[NUM_ENEMIES]     = { -13,  -6, -10,  -8 };

const int collision_damage = 8;

const int col_x_br                 = 8;
const int col_t_br                 = 12;
const int col_b_br                 = 28;
const int col_x_enemy[NUM_ENEMIES] = {  2,  6,  8,  10 };
const int col_t_enemy[NUM_ENEMIES] = {  2, 12, 10,  10 };
const int col_b_enemy[NUM_ENEMIES] = { 30, 30, 30,  30 };

/**********************************************************************/
// Weapons Definition
#define NUM_WEAPONS 9

#define RING    0
#define EPSILON 1
#define MELODY  2
#define MU      3
#define NABLA   4
#define NEKOPAN 5
#define PNP     6
#define PUMP    7
#define ZAYAKU  8

#define E_M     999
//                                       RNG EPS MLD MU  NBL NKP PNP PMP ZYK
const int weapon_damage[NUM_WEAPONS] = {  2,  1,  2,  1,  4,  1,  3,  4,  1 };
const int weapon_v_x[NUM_WEAPONS]    = {  2,  0,  0,  0,  2,  3,  4,  3,  4 };
const int weapon_v_y[NUM_WEAPONS]    = {-12,  2,  0,  2,  0,  0,-10,  0,  0 };
const int weapon_drop[NUM_WEAPONS]   = {  1,  0,  0,  0,  0,  0,  1,  0,  0 };
const int weapon_fall[NUM_WEAPONS]   = {  0,  1,  0,  1,  0,  0,  0,  0,  0 };
const int weapon_aim[NUM_WEAPONS]    = {  0,  0,  0,  0,  0,  0,  0,  1,  0 };
const int weapon_multi[NUM_WEAPONS]  = {  0,  0,  1,  0,  0,  0,  0,  0,  0 };
const int weapon_invert[NUM_WEAPONS] = {  0,  0,  0,  0,  1,  1,  1,  0,  0 };
const int weapon_random[NUM_WEAPONS] = {  0,  0,  0,  0,  0,  0,  0,  0,  1 };

#define NUM_FALL_SHOT 2
const int fall_shot_x[NUM_FALL_SHOT] = { D_WIDTH/4 - W_WIDTH/2 - 10, 3*D_WIDTH/4 - W_WIDTH/2 + 10 };
const int y_fall = wall_thickness + 2;

#define NUM_MULTI_SHOT 3
const int multi_shot_v_x[NUM_MULTI_SHOT] = { 2,  1,  0 };
const int multi_shot_v_y[NUM_MULTI_SHOT] = { 0, -1, -2 };

/**********************************************************************/
// enemy routine
// WAIT
#define WW 111

// TURN
#define TT 222

// JUMP
#define JJ 333

#define ROUTINE_LENGTH 16
int routine[NUM_ENEMIES][ROUTINE_LENGTH] = {
	{WW,      TT,      NABLA,   WW,      TT,      JJ,     WW,    TT,
	 E_M,     WW,      TT,      JJ,      WW,      TT,     NABLA, TT },
	{WW,      TT,      JJ,      JJ,      TT,      JJ,     JJ,    TT,
	 NEKOPAN, NEKOPAN, NEKOPAN, NEKOPAN, TT,      WW,     RING,  JJ },
	{WW,      TT,      ZAYAKU,  ZAYAKU,  ZAYAKU, ZAYAKU,  WW,    TT,
	 JJ,      TT,      ZAYAKU,  ZAYAKU,  ZAYAKU, ZAYAKU,  WW,    MELODY },
	{TT,      E_M,     JJ,      TT,      PNP,     PNP,    TT,    PNP,
	 PNP,     PUMP,    PUMP,    TT,      PUMP,    PUMP,   TT,    JJ }
};

/**********************************************************************/
#define NUM_PIC_TIUN 2
#define NUM_TIUN 12
const int tiun_v_x[NUM_TIUN] = { -4, -3,  0,  3,  4,  4,  0, -3, -2,  0,  2,  0 };
const int tiun_v_y[NUM_TIUN] = {  0,  3,  4,  3,  0, -3, -4, -3,  0,  2,  0, -2 };
      int tiun_x[NUM_TIUN];
      int tiun_y[NUM_TIUN];
      int tiun_c_x;
      int tiun_c_y;
const int tiun_length = 100;
      int dead_time;
