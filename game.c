#include <stdio.h>
#include <stdlib.h>
#include "gba.h"
#include "game.h"
#include "images/forest.h"
#include "images/heart.h"
#include "images/gameover.h"
//#include "images/.h"
                    /* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"


                    /* TODO: */
// Add any additional states you need for your app.
typedef enum {
    START,
    PLAY,
    //WIN,
    LOSE,
} GBAState;

typedef enum direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
} Direction;


int main(void) {
                    /* TODO: */
    // Manipulate REG_DISPCNT here to set Mode 3. //
	REG_DISPCNT = MODE3 | BG2_ENABLE; 


    // Save current and previous state of button input.
    u32 previousButtons = BUTTONS;
    u32 currentButtons = BUTTONS;

    // Load initial game state
    GBAState state = START;

	
	int size = 10;
	int xd = 1;
	int yd = 1;

	Snake s;
	s.head_X = 120;
	s.head_Y = 80;
	s.tail_X = 120;
	s.tail_Y = 80;
	s.lives = 3;
	s.size = 1;
	s.score = 0;

	Food f;
	f.x = randint(12, 218);
	f.y = randint(12, 139);

	Direction d = UP;

	char buffer[128];

	
    while (1) {
        currentButtons = BUTTONS; // Load the current state of the buttons
		waitForVBlank();

		//checks if a button has been pressed
		if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
			vBlankCounter = 0;
			state = START;
		}
		//sets up the start screen and if the button is pressed, moves onto the next screen
        switch(state) {
            case START:
				s.head_X = 120;
				s.head_Y = 80;
				s.tail_X = 120;
				s.tail_Y = 80;
				s.lives = 3;
				s.size = 1;
				s.score = 0;
				if (vBlankCounter == 1) {
					drawFullScreenImageDMA(forest);
					drawString(5, 80, "Welcome to Snake! Press START to begin.", BLACK);
				}
				if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
					state = PLAY;
					fillScreenDMA(BLACK);
					snprintf(buffer, 128, "Score: %d", s.score);
					drawString(0, 0, buffer, WHITE);
					drawRectDMA(10, 10, 220, 140, RED);
					drawRectDMA(12, 12, 216, 136, BLACK);
					drawImageDMA(WIDTH - 60, 0, HEART_WIDTH, HEART_HEIGHT, heart);
					drawImageDMA(WIDTH - 40, 0, HEART_WIDTH, HEART_HEIGHT, heart);
					drawImageDMA(WIDTH - 20, 0, HEART_WIDTH, HEART_HEIGHT, heart);
				}
                break;
			//the game itself, sets you up with a block moving up and a food item, use the arrows to change direction and if you
			//hit a block, you gain a point. If you hit the boundary, you lose a life.
            case PLAY:
				if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
					d = RIGHT;
				}
				if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
					d = LEFT;
				}
				if (KEY_DOWN(BUTTON_UP, currentButtons)) {
					d = UP;
				}
				if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
					d = DOWN;
				}

				drawRectDMA(s.head_X, s.head_Y, size, size, GREEN);
				drawRectDMA(f.x, f.y, size, size, RED);
				switch (d) {
					case RIGHT:
						if ((((f.x <= s.head_X) && (s.head_X <= f.x + size)) && ((f.y <= s.head_Y) && (s.head_Y <= f.y + size)))
							|| (((f.x <= s.head_X + size) && (s.head_X + size <= f.x + size)) && ((f.y <= s.head_Y + size) && (s.head_Y + size <= f.y + size)))) {
							drawRectDMA(f.x, f.y, size, size, BLACK);
							f.x = 0;
							f.y = 0;
							drawRectDMA(0, 0, 170, 10, BLACK);
							s.score = s.score + 1;
							snprintf(buffer, 128, "Score: %d", s.score);
							drawString(0, 0, buffer, WHITE);
							s.size = s.size + 1;
							f.x = randint(12, 218);
							f.y = randint(12, 139);

						}
						drawRectDMA(s.head_X, s.head_Y, size, size, BLACK);
						s.head_X = s.head_X + xd;
						if ((s.head_X == 218) || (s.head_X == 12)) {
							drawRectDMA(WIDTH - (20 * s.lives), 0, size, size, BLACK);
							s.lives = s.lives - 1;
							d = UP;
							if (s.lives == 0) {
								state = LOSE;
								fillScreenDMA(BLACK);
								drawFullScreenImageDMA(gameover);
							}
							s.head_X = 120;
							s.head_Y = 80;
						}
						else {
							drawRectDMA(s.head_X, s.head_Y, size, size, GREEN);
						}
						break;
					case LEFT:
						if ((((f.x <= s.head_X) && (s.head_X <= f.x + size)) && ((f.y <= s.head_Y) && (s.head_Y <= f.y + size)))
							|| (((f.x <= s.head_X + size) && (s.head_X + size <= f.x + size)) && ((f.y <= s.head_Y + size) && (s.head_Y + size <= f.y + size)))) {
							drawRectDMA(f.x, f.y, size, size, BLACK);
							f.x = 0;
							f.y = 0;
							s.score = s.score + 1;
							drawRectDMA(0, 0, 170, 10, BLACK);
							snprintf(buffer, 128, "Score: %d", s.score);
							drawString(0, 0, buffer, WHITE);
							s.size = s.size + 1;
							f.x = randint(12, 218);
							f.y = randint(12, 139);
						}
						drawRectDMA(s.head_X, s.head_Y, size, size, BLACK);
						s.head_X = s.head_X - xd;
						if ((s.head_X == 218) || (s.head_X == 12)) {
							drawRectDMA(WIDTH - (20 * s.lives), 0, size, size, BLACK);
							s.lives = s.lives - 1;
							d = UP;
							if (s.lives == 0) {
								state = LOSE;
								fillScreenDMA(BLACK);
								drawFullScreenImageDMA(gameover);
							}
							s.head_X = 120;
							s.head_Y = 80;
						}
						else {
							drawRectDMA(s.head_X, s.head_Y, size, size, GREEN);
						}
						break;
					case UP:
						if ((((f.x <= s.head_X) && (s.head_X <= f.x + size)) && ((f.y <= s.head_Y) && (s.head_Y <= f.y + size)))
							|| (((f.x <= s.head_X + size) && (s.head_X + size <= f.x + size)) && ((f.y <= s.head_Y + size) && (s.head_Y + size <= f.y + size)))) {
							drawRectDMA(f.x, f.y, size, size, BLACK);
							f.x = 0;
							f.y = 0;
							s.score = s.score + 1;
							drawRectDMA(0, 0, 170, 10, BLACK);
							snprintf(buffer, 128, "Score: %d", s.score);
							drawString(0, 0, buffer, WHITE);
							s.size = s.size + 1;
							f.x = randint(12, 218);
							f.y = randint(12, 139);

						}
						drawRectDMA(s.head_X, s.head_Y, size, size, BLACK);
						s.head_Y = s.head_Y - yd;
						if ((s.head_Y == 12) || (s.head_Y == 138)) {
							drawRectDMA(WIDTH - (20 * s.lives), 0, size, size, BLACK);
							s.lives = s.lives - 1;
							if (s.lives == 0) {
								state = LOSE;
								fillScreenDMA(BLACK);
								drawFullScreenImageDMA(gameover);
							}
							s.head_X = 120;
							s.head_Y = 80;
						}
						else {
							drawRectDMA(s.head_X, s.head_Y, size, size, GREEN);
						}
						break;
					case DOWN:
						if ((((f.x <= s.head_X) && (s.head_X <= f.x + size)) && ((f.y <= s.head_Y) && (s.head_Y <= f.y + size)))
							|| (((f.x <= s.head_X + size) && (s.head_X + size <= f.x + size)) && ((f.y <= s.head_Y + size) && (s.head_Y + size <= f.y + size)))) {
							drawRectDMA(f.x, f.y, size, size, BLACK);
							f.x = 0;
							f.y = 0;
							s.score = s.score + 1;
							drawRectDMA(0, 0, 170, 10, BLACK);
							snprintf(buffer, 128, "Score: %d", s.score);
							drawString(0, 0, buffer, WHITE);
							s.size = s.size + 1;
							f.x = randint(12, 218);
							f.y = randint(12, 139);

						}
						drawRectDMA(s.head_X, s.head_Y, size, size, BLACK);
						s.head_Y = s.head_Y + yd;
						if ((s.head_Y == 12) || (s.head_Y == 138)) {
							drawRectDMA(WIDTH - (20 * s.lives), 0, size, size, BLACK);
							s.lives = s.lives - 1;
							d = UP;
							if (s.lives == 0) {
								state = LOSE;
								fillScreenDMA(BLACK);
								drawFullScreenImageDMA(gameover);
							}
							s.head_X = 120;
							s.head_Y = 80;
						}
						else {
							drawRectDMA(s.head_X, s.head_Y, size, size, GREEN);
						}
						break;
				}
				break;
            case LOSE:
				break;
        }



        previousButtons = currentButtons; // Store the current state of the buttons
    }

    //UNUSED(previousButtons); // You can remove this once previousButtons is used

    return 0;
}