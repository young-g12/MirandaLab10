#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <stdio.h>
#include "Sprite.h"

#include <iostream>
using namespace std;

sprite::sprite()
{
	x = rand() % 600;
	y = rand() % 440;

	xspeed = rand() % 5 + 1;
	yspeed = rand() % 5 + 1;

	xdelay = 0;
	ydelay = 0;

	xcount = 0;
	ycount = 0;

	animdir = 1;
	red = 255;
	green = 255;
	blue = 255;
	frozen = false;
	freezeStartTime = 0;
	angle = 0;

	scale = 1.0f;
	babyStartTime = 0;
	babyShrinking = false;
	alive = true;
}

void sprite::drawSprite()
{
	if (!alive)
		return;

	if (type == SPIN)
	{
		al_draw_tinted_rotated_bitmap(
			image[curframe],
			al_map_rgb(red, green, blue),
			width / 2,
			height / 2,
			x + width / 2,
			y + height / 2,
			angle,
			0
		);
	}
	else if (type == BABY)
	{
		al_draw_tinted_scaled_bitmap(
			image[curframe],
			al_map_rgb(red, green, blue),
			0,
			0,
			width,
			height,
			x,
			y,
			width * scale,
			height * scale,
			0
		);
	}
	else
	{
		al_draw_tinted_bitmap(
			image[curframe],
			al_map_rgb(red, green, blue),
			x,
			y,
			0
		);
	}
}

void sprite::updatesprite()
{
	if (!alive)
		return;

	if (frozen)
	{
		if (al_get_time() - freezeStartTime > 5)
		{
			frozen = false;
		}
		else
		{
			return;
		}
	}

	if (type == BABY && babyShrinking)
	{
		if (al_get_time() - babyStartTime > 10)
		{
			babyShrinking = false;
		}
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

	if (type == SPIN)
	{
		angle += 0.05;
	}
}

void sprite::bouncesprite(int SCREEN_W, int SCREEN_H)
{
	//simple screen bouncing behavior
	if (x < 0)
	{
		x = 0;
		xspeed = rand() % 2 + 4;
		animdir *= -1;
	}

	else if (x > SCREEN_W - width)
	{
		x = SCREEN_W - width;
		xspeed = rand() % 2 - 6;
		animdir *= -1;
	}

	if (y < 0)
	{
		y = 0;
		yspeed = rand() % 2 + 4;
		animdir *= -1;
	}

	else if (y > SCREEN_H - height)
	{
		y = SCREEN_H -  height;
		yspeed = rand() % 2 - 6;
		animdir *= -1;
	}

}

void sprite::load_animated_sprite(int size)
{
	//load the animated sprite
	char s[80];
	maxframe=size;
	type = static_cast<SpriteType>(rand() % 4);
	for (int n=0; n<size; n++)
	{
		sprintf_s(s, "Fairy%d_128.png", n);
		image[n] = al_load_bitmap(s);
	}  
	width=al_get_bitmap_width(image[0]);
	height=al_get_bitmap_height(image[0]);
	curframe = 0;
	framedelay = 5;
	framecount = 0;


}

sprite::~sprite()
{
	for(int i = 0; i < maxframe; i++)
		al_destroy_bitmap(image[i]);
}

bool sprite::collides(sprite& other)
{
	return (
		x < other.x + other.width &&
		x + width > other.x &&
		y < other.y + other.height &&
		y + height > other.y
		);
}

void sprite::handleCollision(int screenW, int screenH)
{
	if (type == SCARED)
	{
		red = rand() % 256;
		green = rand() % 256;
		blue = rand() % 256;

		x = rand() % (screenW - width);
		y = rand() % (screenH - height);
	}

	if (type == FREEZE)
	{
		frozen = true;
		freezeStartTime = al_get_time();
	}

	if (type == BABY)
	{
		scale /= 2.0f;

		x = rand() % (screenW - width);
		y = rand() % (screenH - height);

		babyShrinking = true;
		babyStartTime = al_get_time();

		if (scale < 0.05f)
		{
			cout << "Baby sprite died" << endl;
			alive = false;
		}
	}
}


