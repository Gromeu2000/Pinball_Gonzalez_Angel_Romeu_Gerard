#pragma once
#include "Module.h"
#include "Animation.h"
#include "ModuleTextures.h"
#include "Globals.h"
#include "p2List.h"
#include "p2Point.h"
#include "Box2D/Box2D/Box2D.h"

#define PLAYER_POS_X 
#define PLAYER_POS_Y 

struct Player
{
	//Player elements
	PhysBody*	left_flipper;				//Polygon that represents the left flipper.
	PhysBody*	left_anchor;				//Static circle to which the left flipper attaches to with a revolution joint.
	PhysBody*	right_flipper;				//Polygon that represents the right flipper.
	PhysBody*	right_anchor;				//Static circle to which the right flipper attaches to with a revolution joint.
	PhysBody*	plunger_base;				//Static Rectangle that stablishes the plunger's base. Will have a spring joint where the plunger-ram will be attached to.
	PhysBody*	plunger_ram;				//Dynamic Rectangle that stablishes the plunger's ram. Will be the one to send the ball inside the board. Will have a spring joint attached to p_base.
	PhysBody*	ball;						//Dynamic circle that will act as the ball.
	
	b2RevoluteJoint* left_Anchor_Joint;
	b2RevoluteJoint* right_Anchor_Joint;
	b2PrismaticJoint* plunger_Joint;

	//Texture
	SDL_Texture*	flipper_Left_tex;
	SDL_Texture*	flipper_Right_tex;
	SDL_Texture*	pokeball_tex;
	SDL_Texture*	diglett_plunger_tex;
	SDL_Texture*	flippers_texture;

	//Rects
	SDL_Rect	flipper_L;
	SDL_Rect	flipper_R;

	//lives
	uint lives;

	bool lastWasRight;
	bool lastWasLeft;
	
	//-----------------------Shapes---------------------
	int Left_Flipper[16] = {
	0, 20,
	0, 6,
	5, 0,
	22, 0,
	45, 3,
	64, 6,
	67, 9,
	67, 14,
	/*64, 17,
	50, 20,
	36, 22,
	22, 25,
	8, 25*/
	};

	int Right_Flipper[16] = {
	68, 20,
	68, 6,
	62, 0,
	45, 0,
	23, 3,
	3, 6,
	0, 9,
	0, 14,
	/*3, 17,
	17, 20,
	31, 23,
	45, 25,
	59, 25*/
	};
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	Player player;
	
	void InitPlayer();
	void CreatePlunger();
	//void CreateFlippers(PhysBody* dynamicB, PhysBody* staticB);

	//Score
	uint score = 0;
	uint maxscore = 0;

	uint lives = 3;
};