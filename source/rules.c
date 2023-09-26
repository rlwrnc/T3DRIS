#pragma once
#include "renderer.c"

typedef enum SquareOrientation SquareOrientation;
typedef enum Movements Movements;

typedef struct SquareTetronimo SquareTetronimo;

enum SquareOrientation {
	SQUARE_UP=1, SQUARE_LEFT,
	SQUARE_FRONT, SQUARE_BACK,
	SQUARE_RIGHT, SQUARE_DOWN
};

enum Movements {
	NO_MOVE, NORTH, SOUTH, WEST, EAST,
	ROLL_CW, ROLL_CCW,
	PITCH_CW, PITCH_CCW,
	YAW_CW, YAW_CCW
};

enum Collisions {
	NEG_X=1, POS_X=2, NEG_Y=4, POS_Y=8, NEG_Z=16, POS_Z=32
};

struct SquareTetronimo {
	u8 active[4][3];	// board indices of the four filled in tetronimo blocks
	vec3 position;
	SquareOrientation orientation;
	u32 collisions;
};

void tet_square_check_collisions(SquareTetronimo *square, u8 game_board[10][20][10])
{
	square->collisions = 0;
	for (int i = 0; i < 4; i++) {
		u32 x = square->active[i][0], y = square->active[i][1], z = square->active[i][2];

		if (x == 0 || game_board[x-1][y][z] != 0) {
			square->collisions |= NEG_X;
		}
		if (x == 9 || game_board[x+1][y][z] != 0)
			square->collisions |= POS_X;
		
		if (y == 0 || game_board[x][y-1][z] != 0)
			square->collisions |= NEG_Y;
		if (y == 19 || game_board[x][y+1][z] != 0)
			square->collisions |= POS_Y;

		if (z == 0 || game_board[x][y][z-1] != 0)
			square->collisions |= NEG_Z;
		if (z == 9 || game_board[x][y][z+1] != 0)
			square->collisions |= POS_Z;
	}
}

// TODO: take a second pass at the movement code; it feels kinda like shit
void tet_move_square(SquareTetronimo *square, Movements movement, u8 game_board[10][20][10])
{
	tet_square_check_collisions(square, game_board);
	if (movement == NORTH && !(square->collisions & NEG_Z)) {
		for (u32 i = 0; i < 4; i++) {
			square->active[i][2]--;
		}

		vec3_add(square->position, square->position, (vec3) {0.0, 0.0, -1.0});
	}

	if (movement == WEST && !(square->collisions & NEG_X)) {
		for (u32 i = 0; i < 4; i++) {
			square->active[i][0]--;
		}
		vec3_add(square->position, square->position, (vec3) {-1.0, 0.0, 0.0});
	}

	if (movement == SOUTH && !(square->collisions & POS_Z)) {
		for (u32 i = 0; i < 4; i++) {
			square->active[i][2]++;
		}
		vec3_add(square->position, square->position, (vec3) {0.0, 0.0, 1.0});
	}

	if (movement == EAST && !(square->collisions & POS_X)) {
		for (u32 i = 0; i < 4; i++) {
			square->active[i][0]++;
		}
		vec3_add(square->position, square->position, (vec3) {1.0, 0.0, 0.0});
	}
}

void tet_rotate_square(SquareTetronimo *square, Movements movement, u8 game_board[10][20][10])
{
}
