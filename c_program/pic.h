#pragma once

// Stop
const char pic_br_stop[P_HEIGHT][P_WIDTH] =
#include "player/br0_int.txt"
;

const char pic_br_stop_shoot[P_HEIGHT][P_WIDTH] =
#include "player/br4_int.txt"
;

// Walk
const char pic_br_walk_0[P_HEIGHT][P_WIDTH] =
#include "player/br2-1_int.txt"
;

const char pic_br_walk_1[P_HEIGHT][P_WIDTH] =
#include "player/br2-2_int.txt"
;

const char pic_br_walk_2[P_HEIGHT][P_WIDTH] =
#include "player/br2-3_int.txt"
;

// Jump
const char pic_br_jump[P_HEIGHT][P_WIDTH] =
#include "player/br1_int.txt"
;

const char pic_br_jump_shoot[P_HEIGHT][P_WIDTH] =
#include "player/br7_int.txt"
;

// Bullet
const char pic_bullet[B_HEIGHT][B_WIDTH] =
#include "weapon/tama_single_int.txt"
;

// Weapon
const char pic_weapon[NUM_WEAPONS][W_HEIGHT][W_WIDTH] = {
	#include "weapon/ring_int.txt"
	,
	#include "weapon/epsilon_int.txt"
	,
	#include "weapon/melody_int.txt"
	,
	#include "weapon/mu_int.txt"
	,
	#include "weapon/nabla2_int.txt"
	,
	#include "weapon/nekopan2_int.txt"
	,
	#include "weapon/pnp2_int.txt"
	,
	#include "weapon/pump_int.txt"
	,
	#include "weapon/zayaku_int.txt"
};

// Enemy
const char pic_enemy[NUM_ENEMIES][3][P_HEIGHT][P_WIDTH] = {
	{
		#include "enemy/uppe4_int.txt"
		,
		#include "enemy/uppe5_int.txt"
		,
		#include "enemy/uppe6_int.txt"
	},{
		#include "enemy/catman_int.txt"
		,
		#include "enemy/catman3_int.txt"
		,
		#include "enemy/catman2_int.txt"
	},{
		#include "enemy/suitman4_int.txt"
		,
		#include "enemy/suitman5_int.txt"
		,
		#include "enemy/suitman6_int.txt"
	},{
		#include "enemy/photonman_int.txt"
		,
		#include "enemy/photonman3_int.txt"
		,
		#include "enemy/photonman2_int.txt"
	}
};

// Background
const char pic_stage[D_HEIGHT][D_WIDTH] =
#include "background/default_stage_int.txt"
;

const char pic_title[D_HEIGHT][D_WIDTH] =
#include "background/title_int.txt"
;

const char pic_select[D_HEIGHT][D_WIDTH] =
#include "background/select_int.txt"
;

const char pic_appear[D_HEIGHT][D_WIDTH] =
#include "background/Appear_int.txt"
;

const char pic_clear[D_HEIGHT][D_WIDTH] =
#include "background/gameclear1_int.txt"
;

const char pic_over[D_HEIGHT][D_WIDTH] =
#include "background/gameover_int.txt"
;

// Tiun-Tiun
const char pic_tiun[NUM_PIC_TIUN][T_HEIGHT][T_WIDTH] = {
	#include "tiun/a_int.txt"
	,
	#include "tiun/b_int.txt"
};
