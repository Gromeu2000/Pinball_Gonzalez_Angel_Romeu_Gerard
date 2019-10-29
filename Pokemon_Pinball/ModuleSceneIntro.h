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
	
	//Animations
	Animation* current_animation;
	Animation ditto;
	Animation dugtrio;
	Animation starmie1;
};
