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
	bool lastWasRight;
	bool lastWasLeft;

	//Texture
	SDL_Texture* flippers_texture;

	//Rects
	SDL_Rect	flipper_L;
	SDL_Rect	flipper_R;

	//Player position
	//iPoint position;

	//lives
	uint lives;
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
};