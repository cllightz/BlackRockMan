#include "def.h"
#include "var.h"
#include "pic.h"
#include "cfont.h"
#include "lib.h"

/**********************************************************************/
inline void knockback( int );
inline void bullet_weapon( void );

/**********************************************************************/
// Battle Loop
inline void battle( void ) {
	int routine_num = 0;

	for ( ; ; counter++ ) {
		putback( pic_stage );
	
		p_br = pic_br_stop;
	
		if ( battle_start ) {
			if ( (counter > last_damage + damage_disable_1) && hp > 0 ) {
				button();
			}
			
			if ( counter >= Enext_routine_time && Ehp > 0 ) {
				int current_routine = routine[enemy_num][routine_num];
				
				if (current_routine == E_M) current_routine = EPSILON;
			
				// Shot
				if ( 0 <= current_routine && current_routine < NUM_WEAPONS ) {
					// !(FALL or MULTI) -> once
					int loop = weapon_fall[current_routine]
					             ? NUM_FALL_SHOT
					             : weapon_multi[current_routine] ? NUM_MULTI_SHOT : 1;
					int i;
					
					for ( i = 0; i < loop; i++ ) {
						current_routine = (current_routine == EPSILON)
					                      ? rand( 2 ) ? EPSILON : MU
						                    : current_routine;
					
						weapons[weapon_next].p = pic_weapon[current_routine];
						
						weapons[weapon_next].damage = weapon_damage[current_routine];
						
						weapons[weapon_next].x = weapon_fall[current_routine]
						                           ? fall_shot_x[i]
						                           : Ex + P_WIDTH/2 - B_WIDTH/2 + Edirection * bullet_x_difference;
            
						weapons[weapon_next].y = weapon_fall[current_routine]
						                           ? y_fall
						                           : Ey + weapon_y_difference_stop[enemy_num];
						
						weapons[weapon_next].v_x = Edirection * ( weapon_random[current_routine]
						                                            ? rand( weapon_v_x[current_routine] ) + 1
						                                            : weapon_multi[current_routine]
						                                                ? multi_shot_v_x[i]
						                                                : weapon_v_x[current_routine]
						                                        );
						                                        
						weapons[weapon_next].v_y = weapon_multi[current_routine]
						                             ? multi_shot_v_y[i]
						                             : weapon_random[current_routine]
						                                 ? -rand( weapon_v_x[current_routine] - 1 )
						                                 : weapon_aim[current_routine]
						                                     ? (y - Ey)/( (x < Ex) ? Ex - x : x - Ex )
						                                     : weapon_v_y[current_routine];
						                                    
						weapons[weapon_next].a_y = weapon_drop[current_routine] * g;
						
						weapons[weapon_next].invert_enable = weapon_invert[current_routine];
						weapons[weapon_next].enable = 1;
		
						weapon_next = (weapon_next + 1) % WEAPON_ARRAY_SIZE;
			
						last_Eshot = counter;
					}
					
					Enext_routine_time = counter + Eshoot_cooldown[enemy_num];
					routine_num = (routine_num + 1) % ROUTINE_LENGTH;
				} else if ( current_routine == WW ) {
					Enext_routine_time = counter + Ewait_interval;
					routine_num = (routine_num + 1) % ROUTINE_LENGTH;
				} else if ( current_routine == TT ) {
					Edirection = x < Ex ? -1 : 1;
					Enext_routine_time = counter + 1;
					routine_num = (routine_num + 1) % ROUTINE_LENGTH;
				} else if ( current_routine == JJ ) {
					Ev_y += Ev_y_0_jump[enemy_num];
					Enext_routine_time = counter + Ejump_interval[enemy_num];
					routine_num = (routine_num + 1) % ROUTINE_LENGTH;
				}
			}
		} else if ( y == y_floor ) {
			battle_start = 1;
		}
		
		motion();
		
		// Standing Graphic
		if ( y != y_floor ) {
			p_br = (C_PUSHED && battle_start) ? pic_br_jump_shoot : pic_br_jump;
		}
	
		// Enemy
		if ( Ehp > 0 ) {
			int enemy_graphic = (counter <= last_Eshot + Eshoot_pose_length)
				                ? 2
				                : ( (Ey > y_floor) ? 1 : 0 );
				                
			putpic( Ex, Ey, P_WIDTH, P_HEIGHT, pic_enemy[enemy_num][enemy_graphic],
			        (Edirection < 0) ? 1 : 0 );
		} else if ( dead_time < 0 ) {
			tiun_c_x = Ex + P_WIDTH/2;
			tiun_c_y = Ey + (col_t_enemy[enemy_num] + col_b_enemy[enemy_num])/2;
			dead_time = counter;
			result = 1;
		}
	
		// BRM
		if ( hp > 0 ) {
			if ( counter > last_damage + damage_disable_2 || (counter / blink_length_damage) % 2 ) {
				putpic( x, y, P_WIDTH, P_HEIGHT, p_br, is_left );
			}
		} else if ( dead_time < 0 ) {
			tiun_c_x = x + P_WIDTH/2;
			tiun_c_y = y + (col_t_br + col_b_br)/2;
			dead_time = counter;
			result = 0;
		}
	
		bullet_weapon();
		
		if ( dead_time >= 0 ) {
			int i = 0;
			
			if ( counter == dead_time ) {
				for ( i = 0; i < NUM_TIUN; i++ ) {
					tiun_x[i] = tiun_c_x;
					tiun_y[i] = tiun_c_y;
				}
			} else if ( counter > dead_time ) {
				for ( i = 0; i < NUM_TIUN; i++ ) {
					tiun_x[i] += tiun_v_x[i];
					tiun_y[i] += tiun_v_y[i];
				}
			}
			
			for ( i = 0; i < NUM_TIUN; i++ ) {
				putpic( tiun_x[i], tiun_y[i], T_WIDTH, T_HEIGHT,
				        pic_tiun[ (counter / blink_length_dead) % NUM_PIC_TIUN ], 0 );
			}
			
			if ( counter > dead_time + tiun_length ) {
				break;
			} 
		}
		
		hp_bar();
	
		render();
	}
}

/**********************************************************************/
int main( void ) {
	initialize();
	title();
	
	// Game Loop
	for ( ; ; ) {
		reset();
	
		select();
		appear();
		battle();
		
		if ( result ) clear();
		else          over();
	}
	
	return 0;
}

/**********************************************************************/
// Bullets & bullet_weapons
void bullet_weapon( void ) {
	int i;
	
	int X = x + P_WIDTH/2;
	int Y = y + (col_t_br + col_b_br)/2;
	
	// Collision
	if ( Ex + col_x_enemy[enemy_num] < X && X < Ex + (P_WIDTH - col_x_enemy[enemy_num]) &&
		   Ey + col_t_enemy[enemy_num] < Y && Y < Ey + col_b_enemy[enemy_num] &&
		   counter >= last_damage + damage_disable_2 &&
		   Ehp > 0 ) {
		hp -= collision_damage;
		knockback( (x < Ex) ? -1 : 1 );
	}

	// Bullet (that player shot)
	for ( i = 0; i < BULLET_ARRAY_SIZE; ++i ) {
		if ( bullets[i].enable ) {
			bullets[i].x += bullets[i].direction * bullet_v_x;
			
			X = bullets[i].x + B_WIDTH/2;
			Y = bullets[i].y + B_HEIGHT/2;
			
			// Hit
			if ( bullets[i].x < x_left_wall_b || x_right_wall_b < bullets[i].x ) {
				bullets[i].enable = 0;
			} else if ( Ex + col_x_enemy[enemy_num] < X && X < Ex + (P_WIDTH - col_x_enemy[enemy_num]) &&
						      Ey + col_t_enemy[enemy_num] < Y && Y < Ey + col_b_enemy[enemy_num] &&
						      Ehp > 0 ) {
				Ehp--;
				bullets[i].enable = 0;
			} else {
				putpic( bullets[i].x, bullets[i].y, B_WIDTH, B_HEIGHT, pic_bullet, 0 );
			}
		}
	}
	
	// Weapon (that enemy shot)
	for ( i = 0; i < WEAPON_ARRAY_SIZE; ++i ) {
		if ( weapons[i].enable ) {
			weapons[i].v_y += weapons[i].a_y;
			weapons[i].y   += weapons[i].v_y;
			weapons[i].x   += weapons[i].v_x;
			
			X = weapons[i].x + W_WIDTH/2;
			Y = weapons[i].y + W_HEIGHT/2;
	
			// Hit
			if ( weapons[i].x < x_left_wall_w || x_right_wall_w < weapons[i].x ||
			     weapons[i].y < y_ceil_w      || y_floor_w      < weapons[i].y ) {
				weapons[i].enable = 0;
			} else if ( x + col_x_br < X && X < x + (P_WIDTH - col_x_br) &&
			            y + col_t_br < Y && Y < y + col_b_br &&
						      counter >= last_damage + damage_disable_2 &&
						      hp > 0 && Ehp > 0) {
				hp -= weapons[i].damage;
				weapons[i].enable = 0;
				knockback( (weapons[i].v_x < 0) ? -1 : 1 );
			} else {
				putpic( weapons[i].x, weapons[i].y, W_WIDTH, W_HEIGHT, weapons[i].p,
				        weapons[i].invert_enable && weapons[i].v_x < 0 );
			}
		}
	}
}

/**********************************************************************/
void knockback( int direction ) {
	v_x += direction * knockback_v_x;
	v_y = -knockback_v_y;
	last_damage = counter;
}

