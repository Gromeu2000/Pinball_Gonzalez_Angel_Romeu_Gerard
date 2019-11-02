#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
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

	// Init pos player
	setBall(PLAYER_POS_X, PLAYER_POS_Y);

	//Load rects
	player.flipper_L.x = 390;
	player.flipper_L.y = 2276;
	player.flipper_L.w = 64;
	player.flipper_L.h = 46;

	player.flipper_R.x = 955;
	player.flipper_R.y = 2276;
	player.flipper_R.w = 66;
	player.flipper_R.h = 45;

	player.destroy_ball = false;

	LOG("Loading player");

	//All Player relevant elements of the pinball
	//player.flippers_texture = App->textures->Load("sprites/Pokemon_Pinball_Board_Spritesheet.png");
	//----------------------------------------TEXTURES----------------------------------------------
	player.pokeball_tex = App->textures->Load("sprites/Ball_38px.png");
	player.flipper_Left_tex = App->textures->Load("sprites/Left_Flipper.png");
	player.flipper_Right_tex = App->textures->Load("sprites/Right_Flipper.png");

	//----------------------------------------PLUNGER----------------------------------------------
	//Diglett Plunger
	player.plunger_ram = App->physics->CreateRectangle(b2_dynamicBody, 469, 750, 34, 15, 0);
	player.plunger_base = App->physics->CreateRectangle(b2_staticBody, 469, 769, 34, 15, 0);

	App->physics->CreatePrismaticJoint(player.plunger_ram, player.plunger_base, player.plunger_Joint);

	//----------------------------------------FLIPPERS----------------------------------------------
	//Flippers
	//Left flipper
	player.left_flipper = App->physics->CreateFlipper(b2_dynamicBody, 143, 708, player.Left_Flipper, 16, 0);				//Creates the right flipper object
	player.left_anchor = App->physics->CreateCircle(b2_staticBody, 158, 722, 7, 0);											//Creates the anchor point of the flipper

	App->physics->CreateRevolutionJoint(player.left_flipper, player.left_anchor, 25, -25, 9, 10, player.left_Anchor_Joint);

	player.right_flipper = App->physics->CreateFlipper(b2_dynamicBody, 300, 716, player.Right_Flipper, 16, 0);
	player.right_anchor = App->physics->CreateCircle(b2_staticBody, 288, 722, 7, 0);

	App->physics->CreateRevolutionJoint(player.right_flipper, player.right_anchor, 25, -25, 60, 10, player.right_Anchor_Joint);
	//App->physics->CreateFlippers(); //Metode de crear flippers pero com a rectangles

	App->scene_intro->board.dynamicBody_List.add(player.left_flipper);
	App->scene_intro->board.dynamicBody_List.add(player.right_flipper);

	
	//--------------------------------------AUDIO AND FONTS--------------------------------------
	//set score to 0
	score = 0;

	//Loading FX
	App->audio->LoadFx("audio/FX/050 Diglett Cry.wav");
	App->audio->LoadFx("audio/FX/100 Voltorb Cry.wav");
	App->audio->LoadFx("audio/FX/121 Starmie Cry.wav");
	App->audio->LoadFx("audio/FX/132 Bellsprout Cry.wav");
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
	App->textures->Unload(player.pokeball_tex);

	return true;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB == App->scene_intro->board.dying_sensor)
	{
		//player.destroy_ball = true;
		//setBall(PLAYER_POS_X, PLAYER_POS_Y);
		player.lives --;

		if (player.lives > 0)
		{
			App->audio->PlayFx(6, 0);
		}
		else
		{
			App->audio->PlayMusic("audio/Songs/Game Over.ogg");
		}

		if (score > maxscore)
		{
			maxscore = score;
		}

	}

	if (bodyB->score != 0)
	{
		score += bodyB->score;
	}

	for (int i = 0; i < 3; i++) // check collision with bouncers
	{
		if (bodyB == App->scene_intro->board.voltorb_sensor[i])
		{
			App->audio->PlayFx(2, 0);
			App->scene_intro->is_bouncer_hit[i] = true;
		}
		
	}

	for (int i = 0; i < 2; i++) //check collision with triangles
	{
		if (bodyB == App->scene_intro->App->scene_intro->board.triangle_sensors[i])
		{
			App->scene_intro->is_triangle_hit[i] = true;
			App->audio->PlayFx(8, 0);
		}
	}

	for (int i = 0; i < 2; i++) //check collision with digletts
	{
		if (bodyB == App->scene_intro->App->scene_intro->board.diglett_sensors[i])
		{
			App->audio->PlayFx(1, 0);
		}
	}

	if (bodyB == App->scene_intro->board.bellsprout_S)
	{
		App->audio->PlayFx(4, 0);
	}

	if (bodyB == App->scene_intro->board.starmie_S)
	{
		App->audio->PlayFx(3, 0);
	}
}

// Update: draw background
update_status ModulePlayer::Update()
{
	//--------------------------------------INPUTS PLAYER--------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		player.right_flipper->body->ApplyTorque({ 150 }, true);

		player.lastWasRight = true;
		player.lastWasLeft = false;


	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		player.right_flipper->body->ApplyTorque({ -150 }, true);
		App->audio->PlayFx(5, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		player.left_flipper->body->ApplyTorque({ -150 }, true);

		player.lastWasLeft = true;
		player.lastWasRight = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		player.left_flipper->body->ApplyTorque({ 150 }, true);
		App->audio->PlayFx(5, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		player.plunger_ram->body->ApplyForce({ 0, 10 }, { 0, 0 }, true);	//Brings down the plunger so it can be seen in ready position.
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		player.plunger_ram->body->ApplyForce({ 0, -500 }, { 0, 0 }, true);	//Releases the plunger, sending the ball in the board.
	}
	
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		player.ball = App->physics->CreateCircle(b2_dynamicBody, App->input->GetMouseX(), App->input->GetMouseY(), 18, 0);
		App->scene_intro->board.dynamicBody_List.add(player.ball);
		App->scene_intro->board.dynamicBody_List.getLast()->data->listener = this;
	}
	
	
	//--------------------------------------DEBUG--------------------------------------
	if (App->physics->debug == false) //Temporal measure to debug. Switches between the pinball map and the objects
	{
		//Load left flipper
		App->renderer->Blit(player.flippers_texture, 146, 710, &player.flipper_L);

		//Load right flipper
		App->renderer->Blit(player.flippers_texture, 238, 710, &player.flipper_R);
	}

	//GAMEPLAY LOGIC-------------------------------------------------------------

	//Reset max score
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		App->audio->PlayFx(7, 0);

		if (score > maxscore)
		{
			maxscore = score;
		}

		//Destroybody
		setBall(PLAYER_POS_X, PLAYER_POS_Y);
		score = 0;

		//Load music
		App->audio->PlayMusic("audio/Songs/Main_Theme.ogg");
	}

	

		//if (player.destroy_ball == true && lives != 0)
		//{
		//	App->physics->world->DestroyBody(player.ball->body);
		//
		//	player.ball = App->physics->CreateCircle(b2_dynamicBody, 448, 752, 18, 0);
		//	App->scene_intro->board.dynamicBody_List.add(player.ball);
		//	App->scene_intro->board.dynamicBody_List.getLast()->data->listener = this;
		//
		//	player.destroy_ball = false;
		//}
	

	return UPDATE_CONTINUE;
}

void ModulePlayer::InitPlayer()
{
	player.lastWasRight = false;
	player.lastWasLeft = false;
}

//This
void ModulePlayer::CreatePlunger()
{
	b2PrismaticJointDef plungerJoint;
}

void ModulePlayer::setBall(uint x, uint y)
{
	player.ball = App->physics->CreateCircle(b2_dynamicBody, x, y, 18, 0);
	player.ball->listener = this;
}




