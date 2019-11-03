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
	InitializePlayer();

	LOG("Loading player");

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	//Unload textures
	App->textures->Unload(player.flippers_texture);
	App->textures->Unload(player.pokeball_tex);
	App->textures->Unload(player.diglett_plunger_tex);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	//--------------------------------------PLAYER INPUTS--------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		player.right_flipper->body->ApplyTorque({ 200 }, true);

		player.lastWasRight = true;
		player.lastWasLeft = false;


	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		player.right_flipper->body->ApplyTorque({ -200 }, true);
		App->audio->PlayFx(5, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		player.left_flipper->body->ApplyTorque({ -200 }, true);

		player.lastWasLeft = true;
		player.lastWasRight = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		player.left_flipper->body->ApplyTorque({ 200 }, true);
		App->audio->PlayFx(5, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		player.plunger_ram->body->ApplyForce({ 0, 10 }, { 0, 0 }, true);	//Brings down the plunger so it can be seen in ready position.
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		player.plunger_ram->body->ApplyForce({ 0, -600 }, { 0, 0 }, true);	//Releases the plunger, sending the ball in the board.
	}
	
	//-----------------------------------------------------GAMEPLAY LOGIC-----------------------------------------------------
	//Combo Score
	if (App->scene_intro->board.is_bouncer_hit[0] == true && App->scene_intro->board.is_bouncer_hit[1] == true && App->scene_intro->board.is_bouncer_hit[2] == true && App->scene_intro->board.is_triangle_hit[0] == true && App->scene_intro->board.is_triangle_hit[1] == true && player.double_ball == false)
	{
		setDittoBall(48, 56);				//Ball appears at Ditto Hole
		score += 1000;
		player.double_ball = true;

		resetTextures();
	}
	
	//Reset game.
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		App->audio->PlayFx(7, 0);

		if (score > maxscore)
		{
			maxscore = score;
		}

		prevscore = score;

		//Destroybody
		App->physics->world->DestroyBody(player.ball->body);
		setBall(PLAYER_POS_X, PLAYER_POS_Y);

		if (player.double_ball == true)
		{
			App->physics->world->DestroyBody(player.ditto_ball->body);
			player.double_ball = false;
		}

		resetTextures();

		score = 0;

		//Load music
		App->audio->PlayMusic("audio/Songs/Main_Theme.ogg");
	}

	//Destruction of the ball and ball respawn at plunger's position.
	if (player.destroy_ball == true && player.lives != 0)
	{
		App->physics->world->DestroyBody(player.ball->body);			//In case combo score has been achieved the ball destroyed will be the extra ball.
		setBall(PLAYER_POS_X, PLAYER_POS_Y);
		
		player.destroy_ball = false;
	}

	//Destruction of the Ditto Ball.
	if (player.double_ball == true && player.destroy_ditto_ball == true)
	{
		App->physics->world->DestroyBody(player.ditto_ball->body);
		player.double_ball = false;
		player.destroy_ditto_ball = false;
	}

	//-----------------------------------------------------DEBUG-----------------------------------------------------
	if (App->physics->debug == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			player.ball = App->physics->CreateCircle(b2_dynamicBody, App->input->GetMouseX(), App->input->GetMouseY(), 18, 0, 0.6f);
			App->scene_intro->board.dynamicBody_List.add(player.ball);
			App->scene_intro->board.dynamicBody_List.getLast()->data->listener = this;
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			player.ditto_ball = App->physics->CreateCircle(b2_dynamicBody, App->input->GetMouseX(), App->input->GetMouseY(), 18, 0, 0.6f);
			App->scene_intro->board.dynamicBody_List.add(player.ditto_ball);
			App->scene_intro->board.dynamicBody_List.getLast()->data->listener = this;
		}
	}
	
	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//Ball has been lost
	if (bodyB == App->scene_intro->board.dying_sensor)
	{
		if (bodyB == player.ditto_ball)
		{
			App->audio->PlayFx(6, 0);
			player.destroy_ditto_ball = true;
		}
		else
		{
			player.lives--;
			player.destroy_ball = true;
			if (player.lives > 0)
			{
				App->audio->PlayFx(6, 0);
			}
			else
			{
				App->audio->PlayMusic("audio/Songs/Game Over.ogg");
			}

			if (player.lives == 0)
			{
				if (score > maxscore)
				{
					maxscore = score;
				}

				prevscore = score;

				resetTextures();

				score = 0;
			}
			
		}
	}

	//If the body that has collided had a score parameter, that value is added to the score.
	if (bodyB->score != 0)
	{
		score += bodyB->score;
	}

	for (int i = 0; i < 3; i++) // check collision with bouncers
	{
		if (bodyB == App->scene_intro->board.voltorb_sensor[i])
		{
			App->audio->PlayFx(2, 0);
			App->scene_intro->board.is_bouncer_hit[i] = true;
		}
	}

	for (int i = 0; i < 2; i++) //check collision with triangles
	{
		if (bodyB == App->scene_intro->App->scene_intro->board.triangle_sensors[i])
		{
			App->scene_intro->board.is_triangle_hit[i] = true;
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

	for (int i = 0; i < 4; i++)
	{
		if (bodyB == App->scene_intro->board.light_sensor[i])
		{
			App->scene_intro->board.is_light_sensor_hit[i] = true;
			App->audio->PlayFx(9, 0);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (bodyB == App->scene_intro->board.toplight_sensor[i])
		{
			App->scene_intro->board.is_top_light_sensor_hit[i] = true;
			App->audio->PlayFx(9, 0);
		}
	}

	if (bodyB == App->scene_intro->board.ditto_hole_sensor)
	{
		App->audio->PlayFx(10, 0);
	}

}

bool ModulePlayer::InitializePlayer()
{
	LoadTextures();
	LoadAudio();
	AddShapes();
	
	// Init pos player
	setBall(PLAYER_POS_X, PLAYER_POS_Y);

	player.destroy_ball			= false;
	player.destroy_ditto_ball	= false;
	player.double_ball			= false;
	player.lastWasRight			= false;
	player.lastWasLeft			= true;

	return true;
}

//Creates/Resets the ball back to the plunger so it can be set in play again. 
void ModulePlayer::setBall(uint x, uint y)
{
	player.ball = App->physics->CreateCircle(b2_dynamicBody, x, y, 18, 0, 0.6f);		//Creates a new ball to play with.
	App->scene_intro->board.dynamicBody_List.add(player.ball);							//Adds the ball to dynamicBody_List so it can be blitted with the texture later.
	player.ball->listener = this;														//Adds a contact listener to the ball so sensors can detect a collision.
}

//Creates the extra ball that appears when the combo score is achieved.
void ModulePlayer::setDittoBall(uint x, uint y)
{
	player.ditto_ball = App->physics->CreateCircle(b2_dynamicBody, x, y, 18, 0, 0.6f);		//Creates the ditto ball.
	App->scene_intro->board.dynamicBody_List.add(player.ditto_ball);						//Adds the ball to dynamicBody_List so it can be blitted with the texture later.
	player.ball->listener = this;															//Adds a contact listener to the ball so sensors can detect a collision.
}

bool ModulePlayer::LoadTextures()
{
	//----------------------------------------TEXTURES----------------------------------------------
	player.pokeball_tex			= App->textures->Load("sprites/Ball_36px.png");
	player.ditto_ball_tex		= App->textures->Load("sprites/Ditto_Ball_36px.png");
	player.flipper_Left_tex		= App->textures->Load("sprites/Left_Flipper.png");
	player.flipper_Right_tex	= App->textures->Load("sprites/Right_Flipper.png");
	player.diglett_plunger_tex	= App->textures->Load("sprites/diglett_plunger.png");

	//-----------------------------------------FONTS----------------------------------------------
	//set score to 0
	score = 0;
	
	//---------------------------------------ANIMATION---------------------------------------
	//Diglett Plunger Rect
	player.diglett_plunger.x = 0;
	player.diglett_plunger.y = 0;
	player.diglett_plunger.w = 42;
	player.diglett_plunger.h = 130;

	return true;
}

void ModulePlayer::resetTextures()
{
	for (int i = 0; i < 3; i++)
	{
		App->scene_intro->board.is_bouncer_hit[i] = { false };
	}

	for (int i = 0; i < 2; i++)
	{
		App->scene_intro->board.is_triangle_hit[i] = { false };
	}

	for (int i = 0; i < 4; i++)
	{
		App->scene_intro->board.is_light_sensor_hit[i] = { false };
	}

	for (int i = 0; i < 4; i++)
	{
		App->scene_intro->board.is_top_light_sensor_hit[i] = { false };
	}
}

bool ModulePlayer::LoadAudio()
{
	//Loading FX
	App->audio->LoadFx("audio/FX/050 Diglett Cry.wav");
	App->audio->LoadFx("audio/FX/voltorb.wav");
	App->audio->LoadFx("audio/FX/121 Starmie Cry.wav");
	App->audio->LoadFx("audio/FX/132 Bellsprout Cry.wav");
	App->audio->LoadFx("audio/FX/Flipper.wav");
	App->audio->LoadFx("audio/FX/Lose Ball.wav");
	App->audio->LoadFx("audio/FX/Restart.wav");
	App->audio->LoadFx("audio/FX/Triangles.wav");
	App->audio->LoadFx("audio/FX/Po.wav");
	App->audio->LoadFx("audio/FX/149 Ditto Cry.wav");

	return true;
}

bool ModulePlayer::AddShapes()
{
	//----------------------------------------PLUNGER----------------------------------------------
	//Diglett Plunger
	player.plunger_ram = App->physics->CreateRectangle(b2_dynamicBody, 469, 750, 34, 22, 0);
	player.plunger_base = App->physics->CreateRectangle(b2_staticBody, 469, 769, 34, 15, 0);

	App->physics->CreatePrismaticJoint(player.plunger_ram, player.plunger_base, player.plunger_Joint);

	//----------------------------------------FLIPPERS----------------------------------------------
	//Left flipper
	player.left_flipper = App->physics->CreateFlipper(b2_dynamicBody, 133, 705, player.Left_Flipper, 16, 0);					//Creates the left flipper object
	player.left_anchor = App->physics->CreateCircle(b2_staticBody, 148, 719, 7, 0);												//Creates the anchor point of the flipper

	App->physics->CreateRevolutionJoint(player.left_flipper, player.left_anchor, 25, -25, 9, 10, player.left_Anchor_Joint);		//Creates the joint that seams together the flipper with its anchor

	player.right_flipper = App->physics->CreateFlipper(b2_dynamicBody, 310, 705, player.Right_Flipper, 16, 0);					//Creates the right flipper object
	player.right_anchor = App->physics->CreateCircle(b2_staticBody, 298, 719, 7, 0);											//Creates the anchor point of the flipper

	App->physics->CreateRevolutionJoint(player.right_flipper, player.right_anchor, 25, -25, 60, 10, player.right_Anchor_Joint);	//Creates the joint that seams together the flipper with its anchor

	App->scene_intro->board.dynamicBody_List.add(player.left_flipper);
	App->scene_intro->board.dynamicBody_List.add(player.right_flipper);

	return true;
}


