#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "PhysBody.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" ) //The path to the SDL library is also in ModuleTextures.cpp

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	//Load rects
	player.flipper_L.x = 390;
	player.flipper_L.y = 2276;
	player.flipper_L.w = 64;
	player.flipper_L.h = 46;

	player.flipper_R.x = 955;
	player.flipper_R.y = 2276;
	player.flipper_R.w = 66;
	player.flipper_R.h = 45;

	LOG("Loading player");

	//ball texture
	player.flippers_texture = App->textures->Load("sprites/Pokemon_Pinball_Board_Spritesheet.png");

	//set score to 0
	score = 0;

	//Loading FX
	App->audio->LoadFx("audio/FX/050 Diglett Cry.wav");
	App->audio->LoadFx("audio/FX/100 Voltorb Cry.wav");
	App->audio->LoadFx("audio/FX/121 Starmie Cry.wav");
	App->audio->LoadFx("audio/FX/132 Ditto Cry.wav");
	App->audio->LoadFx("audio/FX/Flipper.wav");
	App->audio->LoadFx("audio/FX/Lose Ball.wav");
	App->audio->LoadFx("audio/FX/Restart.wav");
	App->audio->LoadFx("audio/FX/Triangles.wav");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	//Unload textures
	App->textures->Unload(player.flippers_texture);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		player.lastWasRight = true;
		player.lastWasLeft = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		player.lastWasLeft = true;
		player.lastWasRight = false;
	}
	
	if (App->physics->debug == false) //Temporal measure to debug. Switches between the pinball map and the objects
	{
		//Load left flipper
		App->renderer->Blit(player.flippers_texture, 146, 710, &player.flipper_L);

		//Load right flipper
		App->renderer->Blit(player.flippers_texture, 238, 710, &player.flipper_R);
	}

	//Reset max score
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		if (score > maxscore)
		{
			maxscore = score;
		}

		//Destroybody
		//Restart pos ball
		score = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		score=score + 1000;
	}

	//If ball out of boundaries
	//if (position.y > SCREEN_HEIGHT + 30) {

	//	//Destroy body

	//	//Restart ball
	//	if (player.score > player.maxscore) {
	//		player.maxscore = player.score;
	//	}

	//	playerscore = 0;
	//}

	return UPDATE_CONTINUE;
}

void ModulePlayer::InitPlayer()
{
	player.lastWasRight = false;
	player.lastWasLeft = false;
}



