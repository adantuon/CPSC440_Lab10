class sprite
{
public: 
	~sprite();
	void updatesprite(int WIDTH, int HEIGHT);
	void bouncesprite(int SCREEN_W, int SCREEN_H);
	void load_animated_sprite(int size, int WIDTH, int HEIGHT);
	void drawSprite();
	int getX(){return x;}
	int getY(){return y;}
	void collision(sprite sprites[], int numSprites, int currIndex, int WIDTH, int HEIGHT);
	bool isDead() { return dead; }

private:
	int x,y;
	int width,height;
	int xspeed,yspeed;
	int xdelay,ydelay;
	int xcount,ycount;
	int curframe,maxframe,animdir;
	int framecount,framedelay;
	bool power[4] = { false }; //0 = Spinning, 1 = Scared, 2 = Baby, 3 = Freeze
	float angle;
	ALLEGRO_COLOR tint;
	bool dead;
	float scale;
	bool frozen;
	int frames;
	bool collided;
	ALLEGRO_BITMAP *image[8];
};