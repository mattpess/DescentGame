/*
Matthew Pessolano
7/11/2019
CPSC 440
Final Project
SpriteSheet.cpp
Purpose: Handle drawing of player sprite, movement, and collision
*/

#ifndef SSC
#define SSC

#include "SpriteSheet.h"


Sprite::Sprite()
{
	image = NULL;
}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}
int collided(int x, int y)
{
	BLKSTR* blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return blockdata->tl;
}

bool endValue(int x, int y)
{

	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 8)
	{
		return true;
	}
	else
		return false;
}

int keyValue(int x, int y)
{

	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	return data->user1;
}

void Sprite::InitSprites(int width, int height)
{
	x = 480;
	y = -10;


	maxFrame = 8;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 50;
	frameHeight = 64;
	animationColumns = 8;
	animationDirection = 1;

	image = al_load_bitmap("adventurer.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;

	if (dir == 1) { //right key
		animationDirection = 1;
		x += 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame - 1)
				curFrame = 1;
		}
	}
	else if (dir == 0) { //left key
		animationDirection = 0;
		x -= 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame - 1)
				curFrame = 1;
		}
	}
	else if (dir == 3) { //up key
		animationDirection = 3;
		y -= 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame - 1)
				curFrame = 1;
		}
	}
	else if (dir == 4) { //down key
		animationDirection = 4;
		y += 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > maxFrame - 1)
				curFrame = 1;
		}
	}
	else //represent that they hit the space bar and that mean direction = 2
		animationDirection = dir;

	//check for collided with foreground tiles
	if (animationDirection == 0)
	{
		if (collided(x, y + frameHeight)) { //collision detection to the left
			x = oldx;
			y = oldy;
		}

	}
	else if (animationDirection == 1)
	{
		if (collided(x + frameWidth, y + frameHeight)) { //collision detection to the right
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 3)
	{
		if (collided(x + 15, y + frameHeight/1.5)) { //collision detection to the up
			x = oldx;
			y = oldy;
		}

	}
	else if (animationDirection == 4)
	{
		if (collided(x + 15, y + frameHeight * 1.1125)) { //collision detection below
			x = oldx;
			y = oldy;
		}
	}
}

bool Sprite::CollisionEndBlock()
{
	if (endValue(x + frameWidth / 2, y + frameHeight + 5))
		return true;
	else
		return false;
}

int Sprite::CollisionKeyBlock() {
	return(keyValue(x + frameWidth / 2, y + frameHeight + 5));
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	// new functionality added to show jump animation in either direction

	if (animationDirection == 1) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
	else if (animationDirection == 0) {
		al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_HORIZONTAL);
	}
	else if (animationDirection == 2) {
		al_draw_bitmap_region(image, 0, 0, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
	}
	// up and down handled with same animation
	else if (animationDirection == 3 || animationDirection == 4) {
		// alternates direction flipped every 10 frames
		if (rotate < 10) {
			al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
			rotate += 1;
		}
		else {
			al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 1);
			rotate = (rotate + 1) % 20;

		}
	}
}




#endif

