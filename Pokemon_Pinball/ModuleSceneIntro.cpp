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
	pinball.background.x = 786;
	pinball.background.y = 8;
	pinball.background.w = 385;
	pinball.background.h = 557;

	pinball.ditto.x = 331;
	pinball.ditto.y = 1189;
	pinball.ditto.w = 53;
	pinball.ditto.h = 91;

	pinball.voltorb_boost.x = 519;
	pinball.voltorb_boost.y = 1171;
	pinball.voltorb_boost.w = 34;
	pinball.voltorb_boost.h = 35;
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
	pinball.background_tex = App->textures->Load("images/Background.png");
	pinball.mid_tex = App->textures->Load("images/Mid_Screen.png");

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//LOAD ANIMATIONS------------------------------------------------
	pinball.starmie1.PushBack({ 312, 1515, 38, 40 });
	pinball.starmie1.PushBack({ 352, 1515, 38, 40 });
	pinball.starmie1.loop = true;
	pinball.starmie1.speed = 0.1;

	pinball.dugtrio_L.PushBack({ 264, 1443, 48, 62 });
	pinball.dugtrio_L.PushBack({ 314, 1443, 48, 62 });
	pinball.dugtrio_L.PushBack({ 364, 1443, 48, 62 });
	pinball.dugtrio_L.PushBack({ 414, 1443, 48, 62 });
	pinball.dugtrio_L.loop = true;
	pinball.dugtrio_L.speed = 0.08;
	
	pinball.dugtrio_R.PushBack({ 645, 1443, 49, 62 });
	pinball.dugtrio_R.PushBack({ 695, 1443, 49, 62 });
	pinball.dugtrio_R.PushBack({ 745, 1443, 49, 62 });
	pinball.dugtrio_R.PushBack({ 795, 1443, 49, 62 });
	pinball.dugtrio_R.loop = true;
	pinball.dugtrio_R.speed = 0.08;

	pinball.diglett_Left_Side.PushBack({ 394, 1511, 24, 30});
	pinball.diglett_Left_Side.PushBack({ 420, 1511, 22, 30 });
	pinball.diglett_Left_Side.PushBack({ 444, 1511, 18, 30 });
	pinball.diglett_Left_Side.loop = true;
	pinball.diglett_Left_Side.speed = 0.06;
	

	pinball.diglett_Right_Side.PushBack({ 670, 1511, 24, 30 });
	pinball.diglett_Right_Side.PushBack({ 644, 1511, 24, 31 });
	pinball.diglett_Right_Side.PushBack({ 695, 1511, 24, 30 });
	pinball.diglett_Right_Side.loop = true;
	pinball.diglett_Right_Side.speed = 0.06;
	
	pinball.pikachu.PushBack({ 341, 1675, 34, 33 });
	pinball.pikachu.PushBack({ 378, 1675, 30, 33 });
	pinball.pikachu.loop = true;
	pinball.pikachu.speed = 0.03;

	pinball.bellsprout.PushBack({ 615, 1203, 59, 74 });
	pinball.bellsprout.PushBack({ 673, 1205, 59, 74 });
	pinball.bellsprout.PushBack({ 731, 1205, 59, 74 });
	pinball.bellsprout.PushBack({ 791, 1207, 59, 74 });
	pinball.bellsprout.loop = true;
	pinball.bellsprout.speed = 0.03;

	pinball.starmie2.PushBack({ 411, 1309, 44, 46 });
	pinball.starmie2.PushBack({ 411, 1309, 44, 46 });
	pinball.starmie2.PushBack({ 411, 1309, 44, 46 });
	pinball.starmie2.PushBack({ 457, 1311, 44, 48 });
	pinball.starmie2.PushBack({ 457, 1311, 44, 48 });
	pinball.starmie2.PushBack({ 457, 1311, 44, 48 });
	pinball.starmie2.loop = true;
	pinball.starmie2.speed = 0.03;

	pinball.flipper_L.PushBack({ 277, 1625, 48, 34 });
	pinball.flipper_L.PushBack({ 327, 1625, 50, 20 });
	pinball.flipper_L.PushBack({ 379, 1613, 48, 32 });
	pinball.flipper_L.loop = false;
	pinball.flipper_L.speed = 0.06;

	pinball.mid_screen.PushBack({ 1103, 10, 98, 66 });
	pinball.mid_screen.PushBack({ 1203, 10, 98, 66 });
	pinball.mid_screen.PushBack({ 1303, 10, 98, 66 });
	pinball.mid_screen.PushBack({ 1403, 10, 98, 66 });
	pinball.mid_screen.PushBack({ 1503, 10, 98, 66 });
	pinball.mid_screen.PushBack({ 1603, 10, 98, 66 });
	pinball.mid_screen.PushBack({ 1703, 10, 98, 66 });
	pinball.mid_screen.PushBack({ 203, 78, 98, 64 });
	pinball.mid_screen.PushBack({ 303, 78, 98, 64 });
	pinball.mid_screen.PushBack({ 403, 78, 98, 64 });
	pinball.mid_screen.PushBack({ 503, 78, 98, 64 });
	pinball.mid_screen.PushBack({ 603, 78, 98, 64 });
	pinball.mid_screen.PushBack({ 703, 78, 98, 64 });
	pinball.mid_screen.PushBack({ 803, 78, 98, 64 });
	pinball.mid_screen.PushBack({ 903, 78, 98, 64 });
	pinball.mid_screen.PushBack({ 1003, 78, 98, 64 });
	pinball.mid_screen.PushBack({ 1103, 78, 98, 64 });
	pinball.mid_screen.PushBack({ 1203, 78, 98, 64 });
	pinball.mid_screen.loop = true;
	pinball.mid_screen.speed = 0.009;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	//Unload textures
	App->textures->Unload(pinball.background_tex);
	App->textures->Unload(pinball.mid_tex);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		PhysBody* Circle = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), PIXELS_TO_METERS(16));
	}

	//TEXTURES-----------------------------------------
	if (App->physics->debug == false) //Temporal measure to debug. Switches between the pinball map and the objects.
	{
		//Load background
		App->renderer->Blit(pinball.background_tex, 0, 0, &pinball.background);

		//Load ditto
		App->renderer->Blit(pinball.background_tex, 3, 43, &pinball.ditto);

		//Load voltorb
		App->renderer->Blit(pinball.background_tex, 150, 193, &pinball.voltorb_boost);
		App->renderer->Blit(pinball.background_tex, 170, 133, &pinball.voltorb_boost);
		App->renderer->Blit(pinball.background_tex, 110, 153, &pinball.voltorb_boost);

		//ANIMATIONS----------------------------------------

		//Little starmie animation
		App->renderer->Blit(pinball.background_tex, -2, 300, &(pinball.starmie1.GetCurrentFrame()));

		//Left dugtrios animation
		App->renderer->Blit(pinball.background_tex, -2, 337, &(pinball.dugtrio_L.GetCurrentFrame()));

		//Right dugtrios animation
		App->renderer->Blit(pinball.background_tex, 271, 337, &(pinball.dugtrio_R.GetCurrentFrame()));

		//Diglett at left side animation
		App->renderer->Blit(pinball.background_tex, 46, 350, &(pinball.diglett_Left_Side.GetCurrentFrame()));

		//Diglett at right side animation
		App->renderer->Blit(pinball.background_tex, 248, 350, &(pinball.diglett_Right_Side.GetCurrentFrame()));

		//Pikachus animation
		App->renderer->Blit(pinball.background_tex, 275, 490, &(pinball.pikachu.GetCurrentFrame()));
		App->renderer->Blit(pinball.background_tex, 13, 490, &(pinball.pikachu.GetCurrentFrame()));

		//Bellsprout animation
		App->renderer->Blit(pinball.background_tex, 210, 160, &(pinball.bellsprout.GetCurrentFrame()));

		//Starmie2 animation
		App->renderer->Blit(pinball.background_tex, 87, 209, &(pinball.starmie2.GetCurrentFrame()));

		//Mid screen animation
		App->renderer->Blit(pinball.mid_tex, 109, 332, &(pinball.mid_screen.GetCurrentFrame()));
		return UPDATE_CONTINUE;
	}
}

void ModuleSceneIntro::AddSceneAnimations()
{
	//ditto.PushBack({331, 1189, 53, 91});
	//ditto.PushBack({ 386, 1203, 53, 91 });
	//ditto.loop = false;
	//ditto.speed = 0.1;

	
}
