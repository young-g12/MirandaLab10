#include <allegro5/allegro.h>

enum SpriteType
{
	SPIN,
	SCARED,
	BABY,
	FREEZE
};

class sprite
{
public: 
	sprite();
	~sprite();
	void updatesprite();
	void bouncesprite(int SCREEN_W, int SCREEN_H);
	void load_animated_sprite(int size);
	void drawSprite();
	float red;
	float green;
	float blue;
	bool collides(sprite& other);
	void handleCollision(int screenW, int screenH);


	int getX(){return x;}
	int getY(){return y;}

private:
	int x,y;
	int width,height;
	int xspeed,yspeed;
	int xdelay,ydelay;
	int xcount,ycount;
	int curframe,maxframe,animdir;
	int framecount,framedelay;
	ALLEGRO_BITMAP *image[9];
	SpriteType type;
};