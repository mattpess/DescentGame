/*
Matthew Pessolano
7/9/2019
CPSC 440
Final Project
SpriteSheet.h
Purpose: Handle drawing of player sprite, movement, and collision.
*/

#ifndef SSH
#define SSH

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "mappy_A5.h"
#include <iostream>
using namespace std;
class Sprite
{
	/*
	Purpose: Handle returning information based on colliding with blocks at a given location
	Parameters: x and y are the current player location
	Output: 1 for colliding into block, 0 otherwise
	*/
	friend int collided(int x, int y);
	/*
	Purpose: Handles if a block is a exit/end-value block
	Parameters: x and y position of the block
	Output: True if exit block, false otherwise
	*/
	friend bool endValue(int x, int y);
public:
	/*
Purpose: Default constructor for Sprite
Parameters: None
Output: Sprite object
*/
	Sprite();
	/*
Purpose: Destructor for Sprite
Parameters: None
Output: Bitmap and object deleted
*/
	~Sprite();
	/*
Purpose: Initialize Sprite starting values
Parameters: Width and height of sprite
Output: None; object variables instantiated
*/
	void InitSprites(int width, int height);
	/*
Purpose: Change sprite position based on current direction of movement
Parameters:Width and height of sprite, dir is the direction of movement
Output: None; sprite object attempted to move
*/
	void UpdateSprites(int width, int height, int dir); //dir 1 = right, 0 = left, 2 = Standing Still
		/*
	Purpose: Sprite posted to screen
	Parameters: offsets are how far from the current x and y value the sprite should be drawn
	Output: None; sprite printed to display
	*/
	void DrawSprites(int xoffset, int yoffset);
	/*
Purpose: Not used in implementation of this code, kept for future implementations
*/
	bool CollideSprite();
	float getX() { return x; }
	float getY() { return y; }
	int getWidth() { return frameWidth; }
	int getHeight() { return frameHeight; }
	void setXY(int newX, int newY) {
		x = newX;
		y = newY;
	}
	/*
Purpose: Checks the end value of the block below the player
Parameters: None, uses member variable
Output: True if end block, false otherwise
*/
	bool CollisionEndBlock();

	int CollisionKeyBlock();

private:
	float x;
	float y;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationRows;
	int animationDirection;
	// handles movement while sprite goes up or down
	int rotate = 0;

	ALLEGRO_BITMAP* image;
};

#endif

