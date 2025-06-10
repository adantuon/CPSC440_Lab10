#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "Sprite.h"

#include <iostream>
using namespace std;

void sprite::drawSprite()
{
	al_draw_tinted_scaled_rotated_bitmap(image[curframe], tint, 32, 32, x, y, scale, scale, angle * 2 * ALLEGRO_PI, 0);
}

void sprite::updatesprite(int WIDTH, int HEIGHT)
{
	if (!dead  && !frozen) {
		//If collision and power teleports on collision, then teleport
		if (collided && (power[1] || power[2])) {
			x = (rand() % (WIDTH - width + 1)) + width / 2;
			y = (rand() % (HEIGHT - height + 1)) + height / 2;
		}

		//update x position
		if (++xcount > xdelay)
		{
			xcount = 0;
			x += xspeed;
		}

		//update y position
		if (++ycount > ydelay)
		{
			ycount = 0;
			y += yspeed;
		}


		if (framecount++ > framedelay)
		{
			framecount = 0;
			curframe++;
			if (curframe >= maxframe)
				curframe = 0;
		}
	}
	//Increment frames since last collision
	frames++;

	//If frozen and 5 seconds have passed, unfreeze
	if (frozen && frames > 300) {
		frozen = false;
	}

	//If not dead and currently scaled down and more than 10 seconds have passed since last collision, return to 1x scale
	if (!dead && scale < 1.0 && frames > 600) {
		scale = 1.0;
	}

	//If power would rotate then increment rotate every frame
	if (power[0]) {
		angle += 0.01;

		if (angle > 1.0) {
			angle -= 1.0;
		}
	}
}

void sprite::bouncesprite(int SCREEN_W, int SCREEN_H)
{
	//simple screen bouncing behavior
	if (x - width / 2 < 0)
	{
		x = width / 2;
		xspeed = rand() % 2 + 4;
		animdir *= -1;
	}

	else if (x + width / 2 > SCREEN_W)
	{
		x = SCREEN_W - width / 2;
		xspeed = rand() % 2 - 6;
		animdir *= -1;
	}

	if (y - height / 2 < 0)
	{
		y = height / 2;
		yspeed = rand() % 2 + 4;
		animdir *= -1;
	}

	else if (y + height / 2 > SCREEN_H)
	{
		y = SCREEN_H -  height / 2;
		yspeed = rand() % 2 - 6;
		animdir *= -1;
	}

}

void sprite::load_animated_sprite(int size, int WIDTH, int HEIGHT)
{
	//load the animated sprite
	char s[80];
	maxframe=size;
	for (int n=0; n<size; n++)
	{
		sprintf_s(s,"banana%d.png",n);
		image[n] = al_load_bitmap(s);
	}  
	width=al_get_bitmap_width(image[0]);
	height=al_get_bitmap_height(image[0]);
	curframe = 0;
	framedelay = 5;
	framecount = 0;

	x = (rand() % (WIDTH - width + 1)) + width / 2;
	y = (rand() % (HEIGHT - height + 1)) + height / 2;

	if (rand() % 2) {
		xspeed = rand() % 2 + 4;
	}
	else {
		xspeed = rand() % 2 - 6;
	}

	if (rand() % 2) {
		yspeed = rand() % 2 + 4;
	}
	else {
		yspeed = rand() % 2 - 6;
	}

	power[rand() % 4] = true;

	angle = 0.0;

	tint = al_map_rgb(255, 255, 255);

	dead = false;
	scale = 1.0;

	frozen = false;

	frames = 0;
	collided = false;
	
}

sprite::~sprite()
{
	for(int i = 0; i < maxframe; i++)
		al_destroy_bitmap(image[i]);
}

void sprite::collision(sprite sprites[], int numSprites, int currIndex, int WIDTH, int HEIGHT) {
	if (!dead && !frozen) {

		for (int i = 0; i < numSprites; i++) {

			if (i != currIndex && !sprites[i].isDead()) {

				if ((x + width / 2 >= sprites[i].getX() - width / 2 && x - width / 2 <= sprites[i].getX() + width / 2) &&
					(y + height / 2 >= sprites[i].getY() - height / 2 && y - height / 2 <= sprites[i].getY() + height / 2))
				{
					frames = 0;

					//If Scared or Baby
					if (power[1] || power[2]) {
						if (power[1]) {
							tint = al_map_rgb(rand() / 256, rand() / 256, rand() / 256);
						}
						else {
							scale /= 2;
							if ((int)(64 * scale) == 0) {
								printf("Sprite Died\n");
								dead = true;
							}
						}
					}

					//If Freeze
					else if (power[3]) {
						frozen = true;
					}

					collided = true;
					return;
				}
			}
		}
		collided = false;
	}
}



