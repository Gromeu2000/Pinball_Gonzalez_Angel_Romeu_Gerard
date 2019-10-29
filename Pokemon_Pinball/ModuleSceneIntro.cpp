#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	background.x = 786;
	background.y = 8;
	background.w = 384;
	background.h = 556;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//Load music
	App->audio->PlayMusic("audio/Songs/Main_Theme.ogg");

	//Load textures
	background_tex = App->textures->Load("images/Background.png");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	//Unload textures
	App->textures->Unload(background_tex);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	//TEXTURES-----------------------------------------

	//Load background
	App->renderer->Blit(background_tex, 0, 0, &background);

	//ANIMATIONS----------------------------------------

	//Little starmie animation
	App->renderer->Blit(background_tex, 0, 0, &(starmie1.GetCurrentFrame()));
	
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::AddSceneAnimations()
{
	ditto.PushBack({331, 1189, 53, 91});
	ditto.PushBack({ 386, 1203, 53, 91 });
	ditto.loop = false;
	ditto.speed = 0.1;

	dugtrio.PushBack({ 331, 1189, 53, 91 });
	dugtrio.PushBack({ 386, 1203, 53, 91 });
	dugtrio.loop = true;
	dugtrio.speed = 0.1;

	starmie1.PushBack({ 312, 1515, 37, 41 });
	starmie1.PushBack({ 352, 1203, 37, 41 });
	starmie1.loop = true;
	starmie1.speed = 0.1;
}
