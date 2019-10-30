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

	ditto.x = 331;
	ditto.y = 1189;
	ditto.w = 53;
	ditto.h = 91;

	voltorb_boost.x = 519;
	voltorb_boost.y = 1171;
	voltorb_boost.w = 34;
	voltorb_boost.h = 35;
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

	//LOAD ANIMATIONS------------------------------------------------
	starmie1.PushBack({ 312, 1515, 38, 40 });
	starmie1.PushBack({ 352, 1515, 38, 40 });
	starmie1.loop = true;
	starmie1.speed = 0.1;

	dugtrio_L.PushBack({ 264, 1443, 48, 62 });
	dugtrio_L.PushBack({ 314, 1443, 48, 62 });
	dugtrio_L.PushBack({ 364, 1443, 48, 62 });
	dugtrio_L.PushBack({ 414, 1443, 48, 62 });
	dugtrio_L.loop = true;
	dugtrio_L.speed = 0.08;
	
	dugtrio_R.PushBack({ 645, 1443, 49, 62 });
	dugtrio_R.PushBack({ 695, 1443, 49, 62 });
	dugtrio_R.PushBack({ 745, 1443, 49, 62 });
	dugtrio_R.PushBack({ 795, 1443, 49, 62 });
	dugtrio_R.loop = true;
	dugtrio_R.speed = 0.08;

	diglett_side_L.PushBack({ 394, 1511, 24, 30});
	diglett_side_L.PushBack({ 420, 1511, 22, 30 });
	diglett_side_L.PushBack({ 444, 1511, 18, 30 });
	diglett_side_L.loop = true;
	diglett_side_L.speed = 0.06;


	diglett_side_R.PushBack({ 670, 1511, 24, 30 });
	diglett_side_R.PushBack({ 644, 1511, 24, 31 });
	diglett_side_R.PushBack({ 695, 1511, 24, 30 });
	diglett_side_R.loop = true;
	diglett_side_R.speed = 0.06;

	pikachu.PushBack({ 341, 1675, 34, 33 });
	pikachu.PushBack({ 378, 1675, 30, 33 });
	pikachu.loop = true;
	pikachu.speed = 0.03;

	bellsprout.PushBack({ 615, 1203, 59, 74 });
	bellsprout.PushBack({ 673, 1205, 59, 74 });
	bellsprout.PushBack({ 731, 1205, 59, 74 });
	bellsprout.PushBack({ 791, 1207, 59, 74 });
	bellsprout.loop = true;
	bellsprout.speed = 0.03;


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

	//Load ditto
	App->renderer->Blit(background_tex, 3, 43, &ditto);

	//Load voltorb
	App->renderer->Blit(background_tex, 150, 193, &voltorb_boost);
	App->renderer->Blit(background_tex, 170, 133, &voltorb_boost);
	App->renderer->Blit(background_tex, 110, 153, &voltorb_boost);

	//ANIMATIONS----------------------------------------

	//Little starmie animation
	App->renderer->Blit(background_tex, -2, 300, &(starmie1.GetCurrentFrame()));

	//Left dugtrios animation
	App->renderer->Blit(background_tex, -2, 337, &(dugtrio_L.GetCurrentFrame()));
	
	//Right dugtrios animation
	App->renderer->Blit(background_tex, 271, 337, &(dugtrio_R.GetCurrentFrame()));

	//Diglett at left side animation
	App->renderer->Blit(background_tex, 46, 350, &(diglett_side_L.GetCurrentFrame()));

	//Diglett at right side animation
	App->renderer->Blit(background_tex, 248, 350, &(diglett_side_R.GetCurrentFrame()));

	//Pikachus animation
	App->renderer->Blit(background_tex, 275, 490, &(pikachu.GetCurrentFrame()));
	App->renderer->Blit(background_tex, 13, 490, &(pikachu.GetCurrentFrame()));

	//Bellsprout animation
	App->renderer->Blit(background_tex, 210, 160, &(bellsprout.GetCurrentFrame()));
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::AddSceneAnimations()
{
	//ditto.PushBack({331, 1189, 53, 91});
	//ditto.PushBack({ 386, 1203, 53, 91 });
	//ditto.loop = false;
	//ditto.speed = 0.1;

	
}
