#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

class PhysBody;
class ModuleSceneIntro;

struct PinballMachine
{
	bool debugMode;
	
	//Textures
	SDL_Texture* background_tex;
	SDL_Texture* mid_tex;

	//Rects
	SDL_Rect background;
	SDL_Rect ditto;
	SDL_Rect ditto_expanded;
	SDL_Rect voltorb_boost;
	SDL_Rect blocker;
	SDL_Rect triangle_boosted_L;
	SDL_Rect triangle_boosted_R;
	SDL_Rect flipper_L;
	SDL_Rect flipper_R;

	//Animations
	Animation dugtrio_R;
	Animation dugtrio_L;
	Animation diglett_Right_Side;
	Animation diglett_Left_Side;
	Animation starmie1;
	Animation pikachu;
	Animation bellsprout;
	Animation starmie2;
	
	Animation mid_screen;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	void AddSceneAnimations(); //Load animations

public:
	PinballMachine pinball;
};
