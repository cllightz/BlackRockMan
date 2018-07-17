#pragma once
#include "def.h"
#include "var.h"

/**********************************************************************/
inline void putback( const char[][D_WIDTH] );
inline void putpic( int, int, int width, int, const char[][width], char );
inline void putABC( int, int, char, char );
inline void putnum( int, int, int, char, char );
inline void msleep( unsigned int );
inline void flush( void );
inline void render( void );

inline void move( int );

inline void title( void );
inline void select( void );
inline void appear( void );
inline void clear( void );
inline void over( void );

inline void button( void );
inline void motion( void );
inline void hp_bar( void );

inline void initialize( void );
inline void reset( void );

inline int rand( int );
inline int power10( int );

/**********************************************************************/
// Button
void button( void ) {
	// center: shot
	if ( C_PUSHED ) {
		p_br = y == y_floor ? pic_br_stop_shoot : pic_br_jump_shoot;

		if ( counter >= last_shot + shoot_interval ) {
			bullets[bullet_next].enable = 1;
			bullets[bullet_next].direction = is_left ? -1 : 1;
			
			bullets[bullet_next].x = x + P_WIDTH/2 - B_WIDTH/2
			                           + bullets[bullet_next].direction * bullet_x_difference;
			                           
			bullets[bullet_next].y = y + bullet_y_difference_stop;
			
			bullet_next = (bullet_next + 1) % BULLET_ARRAY_SIZE;
			last_shot = counter;
		}
	}
	
	// left
	if (  L_PUSHED && !C_PUSHED && !R_PUSHED ) {
		move( -1 );
	}

	// right
	if ( !L_PUSHED && !C_PUSHED &&  R_PUSHED ) {
		move( +1 );
	}

	// left + right: jump
	if (  L_PUSHED &&  R_PUSHED ) {
		if ( y == y_floor ) {
			v_y = v_y_0_jump;
		}
	}
}

// Move
void move( int k_v_x ) {
	x += k_v_x * v_x_0;
	if (x < x_left_wall)  x = x_left_wall;
	if (x > x_right_wall) x = x_right_wall;
	
	switch ( (counter / move_frame_length) % 4 ) {
		case 0:
			p_br = pic_br_walk_0;
			break;
			
		case 1:
		case 3:
			p_br = pic_br_walk_1;
			break;
			
		case 2:
			p_br = pic_br_walk_2;
			break;
	}
	
	is_left = k_v_x < 0;
}

// Motion
void motion( void ) {
	// Gravity Effect
	// Player
	if ( v_y < 0 || y < y_floor ) {
		v_y += g;
		y   += v_y;
	}

	if ( y < y_ceil ) {
		v_y = 0;
		y = y_ceil;
	}

	if ( y_floor < y ) {
		v_y = 0;
		y = y_floor;
	}
	
	// Enemy
	if ( Ev_y < 0 || v_y < y_floor ) {
		Ev_y += g;
		Ey   += Ev_y;
	}
	
	if ( Ey < y_ceil ) {
		Ev_y = 0;
		Ey = y_ceil;
	}

	if ( y_floor < Ey ) {
		Ev_y = 0;
		Ey = y_floor;
	}
	
	if ( Ey < y_floor ) {
		Ex += Edirection * Ev_x_0[enemy_num];
		
		if (Ex < x_left_wall)  Ex = x_left_wall;
		if (Ex > x_right_wall) Ex = x_right_wall;
	}
	
	// Knock Back Effect
	x += v_x;
	
	if ( v_x ) {
		v_x += (v_x < 0) ? knockback_decay : -knockback_decay;
	}
	
	if ( x < x_left_wall ) {
		x = x_left_wall;
		v_x = 0;
	}
	
	if ( x_right_wall < x ) {
		x = x_right_wall;
		v_x = 0;
	}
}

// Render Hit Point Bar
void hp_bar( void ) {
	int X, Y, i = 0;
	
	for ( Y = 2; Y < 2 + 1 + hp_bar_num * 2 + 2; Y++ ) {
		for ( X = 2; X < 2 + hp_bar_width + 2; X++ ) {
			*((char*)DISPLAY + X + (Y * D_WIDTH)) = BLACK;
		}
	}
	
	for ( i = 1; i <= hp / (hp_max / hp_bar_num); i++ ) {
		for ( X = 2 + 2; X < 2 + hp_bar_width; X++ ) {
			*((char*)DISPLAY + X + (((hp_bar_num - i) * 2 + 4) * D_WIDTH)) = WHITE;
		}
	}
}

/**********************************************************************/
// Title Screen
void title( void ) {
	putback( pic_title );
	render();
	while ( !C_PUSHED );
}

// Select Screen
void select( void ) {
	int cnt;
	const int interval = 10;

	msleep( 250 );
	// Select Loop
	for ( cnt = 0; ; cnt++ ) {
		putback( pic_select );
		
		putpic( 16, 16, P_WIDTH, P_HEIGHT, pic_enemy[0][(enemy_num == 0 && (cnt/20)%2 - 1)?2:0], 0 );
		putpic( 80, 16, P_WIDTH, P_HEIGHT, pic_enemy[1][(enemy_num == 1 && (cnt/20)%2 - 1)?2:0], 0 );
		putpic( 16, 80, P_WIDTH, P_HEIGHT, pic_enemy[2][(enemy_num == 2 && (cnt/20)%2 - 1)?2:0], 0 );
		putpic( 80, 80, P_WIDTH, P_HEIGHT, pic_enemy[3][(enemy_num == 3 && (cnt/20)%2 - 1)?2:0], 0 );
		
		render();
		
		if ( C_PUSHED && cnt >= interval ) {
			break;
		}
			
		if ( L_PUSHED && cnt >= interval ) {
			enemy_num = (enemy_num == 0) ? NUM_ENEMIES - 1 : enemy_num - 1;
			cnt = 0;
		}
			
		if ( R_PUSHED && cnt >= interval ) {
			enemy_num = (enemy_num + 1) % NUM_ENEMIES;
			cnt = 0;
		}
	}
}

// Appear Screen
void appear( void ) {
	int tmp_x = D_WIDTH/2 - P_WIDTH/2;
	int tmp_y;
	int tmp_v_y = 0;
	int i;

	for ( tmp_y = 0 - P_HEIGHT; ; ) {
		putback( pic_appear );
		
		putpic( tmp_x, tmp_y, P_WIDTH, P_HEIGHT, pic_enemy[enemy_num][JUMP], 1 );
		
		render();
		
		tmp_v_y += g;
		tmp_y   += tmp_v_y;
		
		if ( D_HEIGHT/2 - P_HEIGHT/2 < tmp_y ) {
			tmp_y = D_HEIGHT/2 - P_HEIGHT/2;
			break;
		}
		
		msleep( 10 );
	}
	
	putback( pic_appear );	
	putpic( tmp_x, tmp_y, P_WIDTH, P_HEIGHT, pic_enemy[enemy_num][STOP], 1 );
	render();
	msleep( 500 );
	putback( pic_appear );	
	putpic( tmp_x, tmp_y, P_WIDTH, P_HEIGHT, pic_enemy[enemy_num][ATTACK], 1 );
	render();
	msleep( 200 );
	putback( pic_appear );	
	putpic( tmp_x, tmp_y, P_WIDTH, P_HEIGHT, pic_enemy[enemy_num][STOP], 1 );
	render();
	msleep( 200 );
	putback( pic_appear );	
	putpic( tmp_x, tmp_y, P_WIDTH, P_HEIGHT, pic_enemy[enemy_num][ATTACK], 1 );
	render();
	msleep( 200 );
	putback( pic_appear );	
	putpic( tmp_x, tmp_y, P_WIDTH, P_HEIGHT, pic_enemy[enemy_num][STOP], 1 );
	render();
	msleep( 200 );
	putback( pic_appear );	
	putpic( tmp_x, tmp_y, P_WIDTH, P_HEIGHT, pic_enemy[enemy_num][ATTACK], 1 );
	render();
	msleep( 200 );
	putback( pic_appear );	
	putpic( tmp_x, tmp_y, P_WIDTH, P_HEIGHT, pic_enemy[enemy_num][STOP], 1 );
	render();
	
	tmp_y = 92;
	
	for ( i = 0; i < NAME_LENGTH; i++ ) {
		if ( enemy_name[enemy_num][i] != ' ' ) {
			tmp_x = D_WIDTH/2
			        - ( ((enemy_num == CATMAN) ? NAME_LENGTH - 1 : NAME_LENGTH) *C_WIDTH)/2
			        + i*C_WIDTH;
		
			putABC( tmp_x, tmp_y, enemy_name[enemy_num][i], WHITE );
			        
			render();
			msleep( 50 );
		}
	}
	
	while ( !C_PUSHED );
}

// Game Clear Screen
void clear( void ) {
	int i, j;
	int nums = 9;
	int score = hp * dead_time * 10;

	putback( pic_clear );
	render();
	msleep( 250 );
	
	for ( i = 0; ; i += 13791 ) {
		if ( i > score ) i = score;
	
		for ( j = 0; j < nums; j++ ) {
			putnum( D_WIDTH/2 - (C_WIDTH*nums)/2 + j*C_WIDTH, 84,
			        (i / power10( nums-j-1 ) ) % 10, WHITE, BLACK );
		}
		
		render();
			
		if ( i == score ) break;
	}
	
	while ( !C_PUSHED );
}

// Game Over Screen
void over( void ) {
	putback( pic_over );
	render();
	msleep( 1000 );
	while ( !C_PUSHED );
}

/**********************************************************************/
// ミリ秒単位でスリープ
void msleep( unsigned int sleep_time ) {
	unsigned int finish_time = (unsigned int) *e_time + sleep_time;
	
	while ( *e_time < finish_time );
}

// バッファをフラッシュ
void flush( void ) {
	int i;
	
	for ( i = 0; i < D_HEIGHT * D_WIDTH / sizeof( uint64 ); i++ ) {
		*((uint64*)DISPLAY + i) = 0xFFFFFFFFFFFFFFFFllu;
	}
}

// バッファからレンダリング
void render( void ) {
	int i;
	
	for ( i = 0; i < D_HEIGHT * D_WIDTH / sizeof( uint64 ); i++ ) {
		*(e_vram + i) = *((uint64*)DISPLAY + i);
	}
}

/**********************************************************************/
// 背景を描画
void putback( const char data[][D_WIDTH] ) {
	int i, j;
	
	for ( i = 0; i < D_HEIGHT; i++ ) {
		for ( j = 0; j < D_WIDTH / sizeof( uint64 ); j++ ) {
			*((uint64*)DISPLAY + j + i * D_WIDTH / sizeof( uint64 ))
				= *((uint64*)data[i] + j);
		}
	}
}

// 画像を描画
void putpic( int x, int y, int width, int height, 
						 const char data[][width], char is_inverted ) {
	int i, j, X, Y;

	if ( is_inverted ) {
		for ( i = 0; i < height; i++ ) {
			for ( j = 0; j < width; j++ ) {
				X = x + j;
				Y = y + i;
			
				if ( 0 <= X && X < D_WIDTH && 0 <= Y && Y <= D_HEIGHT ) {
					char pixel = data[i][width - j - 1];
			
					if ( pixel < 8 ) {
						*((char*)DISPLAY + X + (y + i) * D_WIDTH) = pixel;
					}
				}
			}
		}
	} else {
		for ( i = 0; i < height; i++ ) {
			for ( j = 0; j < width; j++ ) {
				X = x + j;
				Y = y + i;
			
				if ( 0 <= X && X < D_WIDTH && 0 <= Y && Y <= D_HEIGHT ) {
					char pixel = data[i][j];
			
					if ( pixel < 8 ) {
						*((char*)DISPLAY + X + (y + i) * D_WIDTH) = pixel;
					}
				}
			}
		}
	}
}

/**********************************************************************/
// 大文字アルファベットを描画
void putABC( int x, int y, char c, char color ) {
	int i, j;
	
	for ( i = 0; i < C_HEIGHT; i++ ) {
		for ( j = 0; j < C_WIDTH; j++ ) {
			if ( e_char[ (int)(c-'A') ][i][j] ) {
				*((char*)DISPLAY + (x + j) + (y + i) * D_WIDTH) = color;
			}
		}
	}
}

/**********************************************************************/
// 数字を描画
void putnum( int x, int y, int num, char color, char background_color ) {
	int i, j;
	
	for ( i = 0; i < C_HEIGHT; i++ ) {
		for ( j = 0; j < C_WIDTH; j++ ) {
			*((char*)DISPLAY + (x + j) + (y + i) * D_WIDTH) = e_number[num][i][j] ? color : background_color;
		}
	}
}

/**********************************************************************/
// Initialize
void initialize( void ) {
	enemy_num = 0;
	rand_seed = 123;
	
	flush();
	render();
}

// Reset before Battle
void reset( void ) {
	int i;
	
	// BRM Spawn Position
	x = x_left_wall + 2;
	y = 0;
	v_x = 0;
	v_y = 0;
	is_left = 0;
	hp = hp_max;
	Ehp = Ehp_max[enemy_num];
	
	// Enemy Spawn Position
	Ex = x_right_wall - 2;
	Ey = y_floor;
	Ev_y = 0;
	Edirection = -1;
	
	for ( i = 0; i < BULLET_ARRAY_SIZE; i++ ) {
		bullets[i].enable = 0;
	}
	
	bullet_next = 0;
	
	for ( i = 0; i < WEAPON_ARRAY_SIZE; i++ ) {
		weapons[i].enable = 0;
	}
	
	weapon_next = 0;
	
	last_shot          = -1000;
	last_Eshot         = -1000;
	last_damage        = -1000;
	Enext_routine_time = -1000;
	dead_time          = -1000;
	
	result = 0;
	counter = 0;
	battle_start = 0;
}

/**********************************************************************/
// Generate Random Number
int rand( int mod ) {
	int res = counter * (int)*e_time * rand_seed + (int)*e_time;
	rand_seed = res;
	return (res < 0) ? (-res)%mod : res%mod;
}

int power10( int arg ) {
	int i;
	int res = 1;
	
	for ( i = 0; i < arg; ++i ) {
		res *= 10;
	}
	
	return res;
}
