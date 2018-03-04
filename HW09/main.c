#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "myLib.h"
#include "background.h"
#include "gameOver.h"
#include "lose.h"
#include "start.h"
#include "hela.h"
#include "hulk.h"
#include "thor.h"
#include "loki.h"
#include "text.h"
#include "poster.h"
#include "characters.h"
#include "main.h"

/**
* Enum for game state
*/
enum GBAState {
	START,
	FIGHT,
	LOSE,
	GAMEOVER
};

/**
* Enum for enemy
*/
enum ENEMYState {
	LOKI,
	HULK,
	HELA
};

// void startGame();
// void fight(enum ENEMYState enemy);
// void loseGame(enum ENEMYState enemy);
// void gameover();

enum GBAState state = START;
enum ENEMYState enemy = LOKI;

/**
* Reset is for loops
* hp_count is for when we change hp
* hp is Thor's hp
*/
int reset = 0;
int hp_count = 1;
int hp = 5;

/**
* where I use structs
*/
objetcLocation thorE;
objetcLocation enemyE;
objetcLocation *thorPointer = &thorE;
objetcLocation *enemyPointer = &enemyE;

/**
* Sets the variables for the structs
*/
void setInitialVariables()
{
	thorPointer->row = 20;
	thorPointer->col = 20;
	thorPointer->health = hp;
	thorPointer->dx = 1;
	thorPointer->dy = 1;
	thorPointer->height = THOR_HEIGHT;
	thorPointer->width = THOR_WIDTH;

	enemyPointer->row = 80;
	enemyPointer->col = 80;
	enemyPointer->health = 1;
	enemyPointer->dx = 1;
	enemyPointer->dy = 1;
	enemyPointer->height = LOKI_HEIGHT;
	enemyPointer->width = LOKI_WIDTH;

	hp_count = 1;
}

/**
* Sets the image when the game ends
*/
void gameover()
{
	// delay(1000);
	//drawImage3(0, 0, 240, 160, gameOver);
	// drawImage3(0, 0, 240, 160, poster);
	// delay(40);
	drawImage3(0, 0, 240, 160, characters);
	delay(750);
	drawImage3(0, 0, 240, 160, poster);
	delay(750);
	drawImage3(0, 0, 240, 160, gameOver);
	char *textEnd = "Press select to restart";
	drawString(50, 50, textEnd, GREEN);
	delay(750);
	enemy = LOKI;
}

/**
* Sets the image and text when you lose.
*/
void loseGame(enum ENEMYState enemy)
{
	fillScreen3(BLACK);
	drawImage3(0, 0, 240, 160, lose);
	delay(100);
	char *textLoki = "Lost to Loki";
	char *textHulk = "Lost to Hulk";
	char *textHela = "Lost to Hela";

	switch (enemy)
	{
		case LOKI:
			drawString(25, 90, textLoki, YELLOW);
			break;
		case HULK:
			drawString(25, 90, textHulk, GREEN);
			break;
		case HELA:
			drawString(25, 90, textHela, RED);
			break;
	}
	delay(100);
	char *cont = "Press Select To Continue";
	drawString(50, 50, cont, BLUE);
	int reset = 0;
	while (!reset)
	{
		if (KEY_DOWN_NOW(BUTTON_SELECT))
		{
      		reset = 1;
    	}
	}
	enemy = LOKI;
	state = START;
}

/**
* Fighting logic
*/
void fight()
{
	// update for thor and the enemy
	if (thorPointer->health == 0) 
	{
		state = LOSE;
		return;
	}
	if (enemyPointer->health == 0) {
		enemyPointer->health = 1;
		updateHearts(3, enemyPointer->health, background);
	}

	// assigns the image based on enemy
	const unsigned short *fighting;
	switch (enemy){
		case LOKI:
			fighting = loki;
			break;
		case HULK:
			fighting = hulk;
			break;
		case HELA:
			fighting = hela;
			break;
	}

	// button logic where drawCopyImage repaints the background
	if (KEY_DOWN_NOW(BUTTON_DOWN))
	{
		if (thorPointer->row < (160-thorPointer->height)) {
			thorPointer->dy = 1;

			thorPointer->oldRow = thorPointer->row;
			thorPointer->oldCol = thorPointer->col;
			//drawImage3(thorPointer->oldRow, thorPointer->col, thorPointer->width, thorPointer->dy, background);


			thorPointer->row = thorPointer->row + thorPointer->dy;

			drawCopyImage(thorPointer->oldRow, thorPointer->oldCol, thorPointer->width, 1, background);
		}
		//drawCopyImage(thorPointer->oldRow, thorPointer->oldCol, thorPointer->width, thorPointer->height, background);
		//drawImage3(thorPointer->row, thorPointer->col, thorPointer->width, thorPointer->height, thor);
	} else if (KEY_DOWN_NOW(BUTTON_UP))
	{
		if (thorPointer->row >= 10) {
			thorPointer->dy = -1;
			thorPointer->oldRow = thorPointer->row;
			thorPointer->oldCol = thorPointer->col;
			thorPointer->row += thorPointer->dy;
			drawCopyImage(thorPointer->oldRow + thorPointer->height, thorPointer->oldCol, thorPointer->width, 1, background);
		}
		

		//drawCopyImage(thorPointer->oldRow, thorPointer->oldCol, thorPointer->width, thorPointer->height, background);

		
		//drawImage3(thorPointer->row, thorPointer->col, thorPointer->width, thorPointer->height, thor);
	} else if (KEY_DOWN_NOW(BUTTON_RIGHT))
	{
		if (thorPointer->col < (240-thorPointer->width)) {
			thorPointer->dx = 1;
			thorPointer->oldRow = thorPointer->row;
			thorPointer->oldCol = thorPointer->col;
			thorPointer->col += thorPointer->dx;
			drawCopyImage(thorPointer->oldRow, thorPointer->oldCol, 1, thorPointer->height, background);
		}
		//drawImage3(thorPointer->row, thorPointer->col, thorPointer->width, thorPointer->height, thor);
		//drawCopyImage(thorPointer->oldRow, thorPointer->oldCol, thorPointer->width, thorPointer->height, background);
	} else if (KEY_DOWN_NOW(BUTTON_LEFT))
	{
		if (thorPointer->col >= 1){
			thorPointer->dx = -1;
			thorPointer->oldRow = thorPointer->row;
			thorPointer->oldCol = thorPointer->col;
			thorPointer->col += thorPointer->dx;
			drawCopyImage(thorPointer->oldRow, thorPointer->oldCol + thorPointer->width, 1, thorPointer->height, background);
		}
		//drawImage3(thorPointer->row, thorPointer->col, thorPointer->width, thorPointer->height, thor);
		//drawCopyImage(thorPointer->oldRow, thorPointer->oldCol, thorPointer->width, thorPointer->height, background);
	} else {
		thorPointer->dx = 0;
		thorPointer->dy = 0;
	}
	drawImage3(thorPointer->row, thorPointer->col, thorPointer->width, thorPointer->height, thor);

	// same logic but for enemy instead of thor
	enemyPointer->oldCol = enemyPointer->col;
	enemyPointer->oldRow = enemyPointer->row;
	enemyPointer->row += enemyPointer->dy;
	enemyPointer->col += enemyPointer->dx;
	if (enemyPointer->row < 10)
	{
		enemyPointer->row = 11;
		enemyPointer->dy = enemyPointer->dy * -1;
		// drawCopyimage
	} else {
		drawCopyImage(enemyPointer->oldRow + enemyPointer->height, enemyPointer->oldCol, enemyPointer->width, 1, background);
	}
	if (enemyPointer->row + enemyPointer->height >= 159)
	{
		enemyPointer->row = 158 - enemyPointer->height;
		enemyPointer->dy = enemyPointer->dy * -1;
	} else {
		drawCopyImage(enemyPointer->oldRow, enemyPointer->oldCol, enemyPointer->width, 1, background);
	}
	if (enemyPointer->col < 1)
	{
		enemyPointer->col = 0;
		enemyPointer->dx = enemyPointer->dx * -1;
	} else {
		drawCopyImage(enemyPointer->oldRow, enemyPointer->oldCol + enemyPointer->width, 1, enemyPointer->height, background);
	}
	if (enemyPointer->col + enemyPointer->width >= 239)
	{
		enemyPointer->col = 238 - enemyPointer->width;
		enemyPointer->dx = enemyPointer->dx * -1;
	} else {
		drawCopyImage(enemyPointer->oldRow, enemyPointer->oldCol, 1, enemyPointer->height, background);
	}
	drawImage3(enemyPointer->row, enemyPointer->col, enemyPointer->width, enemyPointer->height, fighting);

	// this checks to see if the two images have intersected anywhere 
	// and does the appropriate logic based on this
	int thorRow0 = thorPointer->row;
	int thorRowMax = thorPointer->row + thorPointer->height;
	int thorCol0 = thorPointer->col;
	int thorColMax = thorPointer->col + thorPointer ->width;

	int enemyRow0 = enemyPointer->row;
	int enemyRowMax = enemyPointer->row + enemyPointer->height;
	int enemyCol0 = enemyPointer->col;
	int enemyColMax = enemyPointer->col + enemyPointer->width;

	int intersect1 = 1;
	int intersect2 = 1;

	if (thorCol0 > enemyColMax || enemyCol0 >thorColMax) {
		intersect1 = 0;
	}
	if (thorRow0 < enemyRowMax || enemyRow0 < thorRowMax) {
		intersect2 = 0;
	}
	if (intersect1 || intersect2) {
		enemyPointer->dy = enemyPointer->dy * -1;
		enemyPointer->dx = enemyPointer->dx * -1;

		// if thor is still during intersect, then it loses points
		// eslse the enemy loses points during intersection
		if (thorPointer->dx == 0 && thorPointer->dy == 0) {
			thorPointer->health = thorPointer->health -1;
			hp_count += 1;
			updateHearts(4, hp_count, background);
		} else {
			enemyPointer->health = 0;
			if (enemy == HELA) {
			state = GAMEOVER;
			}
			if (enemy == HULK) {
				enemy = HELA;
			}
			if (enemy == LOKI) {
				enemy = HULK;
			}
		}
	}
}

/**
* draws the imaage
*/
void startGame()
{
	drawImage3(0, 0, 240, 160, start);
	char *str = "Press START";
	drawString(10, 90, str, BLUE);
	while (1)
	{
		if (KEY_DOWN_NOW(BUTTON_START))
		{
			delay(10);
			break;
		}
	}
	// draws the appropriate variables
	state = FIGHT;
	drawImage3(0, 0, 240, 160, background);
	setInitialVariables();
	drawImage3(thorPointer->row, thorPointer->col, thorPointer->width, thorPointer->height, thor);
	drawImage3(enemyPointer->row, enemyPointer->col, enemyPointer->width, enemyPointer->height, loki);
	updateHearts(2, enemyPointer->health, background);
	updateHearts(1, thorPointer->health, background);
}

int main(void) {
	// GBA code
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	while (1)
	{
		waitForVblank();
		// this below lets you go back to start at any point
		if (KEY_DOWN_NOW(BUTTON_SELECT))
		{
			state = START;
		}
		switch(state)
		{
			case START:
				startGame();
				break;
			case FIGHT:
				fight();
				break;

			case LOSE:
				loseGame(enemy);
				break;

			case GAMEOVER:
				gameover();
				break;
		}
	}
	return 0;
}



