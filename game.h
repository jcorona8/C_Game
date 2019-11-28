#ifndef GAME_H
#define GAME_H

#include "gba.h"

                    /* TODO: */

            // Create any necessary structs //

typedef struct snake {
	int size;
	char direction;
	char prev_direction;
	int tail_X;
	int tail_Y;
	int head_X;
	int head_Y;
	int lives;
	int score;

} Snake;

typedef struct food {
   int x;
   int y;
} Food;


#endif
