#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro;

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

	//Textures
	SDL_Texture* background_tex;

	//Rects
	SDL_Rect background;
	SDL_Rect ditto;
	SDL_Rect ditto_expanded;
	SDL_Rect voltorb_boost;

	//Animations
	Animation dugtrio_R;
	Animation dugtrio_L;
	Animation diglett_side_R;
	Animation diglett_side_L;
	Animation starmie1;
	Animation pikachu;
	Animation bellsprout;
	
};
