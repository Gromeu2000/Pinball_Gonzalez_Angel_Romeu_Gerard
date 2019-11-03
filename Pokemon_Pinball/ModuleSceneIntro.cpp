#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
#include "ModuleFonts.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	SetAnimationRectPosition();
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	InitializeBoard();

	//Load Shapes:
	//----------------------------Static Shapes----------------------------
	//Static Chains:
	//Background Chains. All chains have their pivot at (0, 0) due to Ric's vector drawing program
	board.background_exterior		= App->physics->CreateChain(b2_staticBody, 0, 0, board.backgroundExterior, 132, 0);
	board.background_interior		= App->physics->CreateChain(b2_staticBody, 0, 0, board.backgroundInterior, 138, 0);
	board.left_kicker				= App->physics->CreateChain(b2_staticBody, 0, 0, board.left_Kicker, 6, 0);
	board.right_kicker				= App->physics->CreateChain(b2_staticBody, 0, 0, board.right_Kicker, 6, 0);
	board.left_arm					= App->physics->CreateChain(b2_staticBody, 0, 0, board.left_Arm, 16, 0);
	board.right_arm					= App->physics->CreateChain(b2_staticBody, 0, 0, board.right_Arm, 16, 0);
	board.topLeft_wall				= App->physics->CreateChain(b2_staticBody, 0, 0, board.topLeft_Wall, 36, 0);
	board.starmie_wall				= App->physics->CreateChain(b2_staticBody, 0, 0, board.starmie_Wall, 38, 0);
	board.centerLeft_miniwall		= App->physics->CreateChain(b2_staticBody, 0, 0, board.centerLeft_MiniWall, 16, 0);
	board.centerRight_miniwall		= App->physics->CreateChain(b2_staticBody, 0, 0, board.centerRight_MiniWall, 18, 0);
	board.bellsprout_wall			= App->physics->CreateChain(b2_staticBody, 0, 0, board.bellsprout_Wall, 86, 0);
	//board.ball_below_flippers		= App->physics->CreateChain(b2_staticBody, 0, 0, board.ball_Below_Flippers, 14, 0);

	//Static Circles (Bouncers)
	board.bouncers[0]			= App->physics->CreateCircle(b2_staticBody, 234, 290, 18, 1);
	board.bouncers[1]			= App->physics->CreateCircle(b2_staticBody, 256, 209, 18, 1);
	board.bouncers[2]			= App->physics->CreateCircle(b2_staticBody, 186, 234, 18, 1);
	board.triangles[0]			= App->physics->CreateRectangle(b2_staticBody, 122, 617, 5, 60, 1.5, -1800 * DEGTORAD);
	board.triangles[1]			= App->physics->CreateRectangle(b2_staticBody, 325, 617, 5, 60, 1.5,  1800 * DEGTORAD);
	board.left_diglett_bouncer	= App->physics->CreateCircle(b2_staticBody, 78, 511, 18, 0.9f);
	board.right_diglett_bouncer = App->physics->CreateCircle(b2_staticBody, 372, 511, 18, 0.9f);


	//SENSORS-------------------------------------------
	board.ditto_hole_sensor = App->physics->CreateCircleSensor(48, 56, 23, 200);

	board.light_sensor[0] = App->physics->CreateRectangleSensor(38, 614, 11, 17, 0, 10);
	board.light_sensor[1] = App->physics->CreateRectangleSensor(83, 614, 11, 17, 0, 10);
	board.light_sensor[2] = App->physics->CreateRectangleSensor(363, 614, 11, 17, 0, 10);
	board.light_sensor[3] = App->physics->CreateRectangleSensor(408, 614, 11, 17, 0, 10);
	
	board.toplight_sensor[0] = App->physics->CreateRectangleSensor(161, 191, 11, 17);
	board.toplight_sensor[1] = App->physics->CreateRectangleSensor(223, 160, 11, 17);
	board.toplight_sensor[2] = App->physics->CreateRectangleSensor(284, 163, 11, 17);

	board.voltorb_sensor[0] = App->physics->CreateCircleSensor(234, 290, 22, 20);
	board.voltorb_sensor[1] = App->physics->CreateCircleSensor(256, 209, 22, 20);
	board.voltorb_sensor[2] = App->physics->CreateCircleSensor(186, 234, 22, 20);

	board.left_wall_sensor = App->physics->CreateRectangleSensor(118, 414, 4, 12, 45, 10);
	board.right_wall_sensor = App->physics->CreateRectangleSensor(330, 412, 4, 12, -45, 10);

	board.triangle_sensors[0] = App->physics->CreateRectangleSensor(126, 621, 5, 60, -1800 * DEGTORAD);
	board.triangle_sensors[1] = App->physics->CreateRectangleSensor(325, 617, 5, 60, 1800 * DEGTORAD);
	
	board.diglett_sensors[0] = App->physics->CreateCircleSensor(84, 511, 20,10);
	board.diglett_sensors[1] = App->physics->CreateCircleSensor(365, 511, 20,10);
	
	board.bellsprout_S = App->physics->CreateCircleSensor(345, 250, 25, 100);

	board.starmie_S = App->physics->CreateCircleSensor(162, 333, 30, 60);

	board.dying_sensor	= App->physics->CreateRectangleSensor(220, 785, 5, 80, 5100 * DEGTORAD);
	
	/*
	board.ball_catcher = App->physics->CreateCircleSensor();
	
	
	board.triangle_sensors[0] = App->physics->CreateCircleSensor();
	board.triangle_sensors[1] = App->physics->CreateCircleSensor();*/


	//OBJECTS---------------------------------------------



	//Load music
	App->audio->PlayMusic("audio/Songs/Main_Theme.ogg");

	//Load textures
	board.background_tex		= App->textures->Load("sprites/Pokemon_Pinball_Board_Spritesheet.png");
	board.mid_tex				= App->textures->Load("sprites/Pokemon_Pinball_Special_Sprites_Spritesheet.png");
	

	//Load fonts 
	score						= App->fonts->Load("sprites/score.png", "0123456789", 1);

	App->renderer->camera.x = App->renderer->camera.y = 0;

	//LOAD ANIMATIONS------------------------------------------------
	AddAnimationPushbacks();


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	//Unload textures
	App->textures->Unload(board.background_tex);
	App->textures->Unload(board.mid_tex);

	/*p2List_item<PhysBody*>* dynBody_iterator = board.dynamicBody_List.getFirst();

	while (dynBody_iterator != NULL)
	{
		board.dynamicBody_List.del(dynBody_iterator);

		dynBody_iterator = dynBody_iterator->next;
	}*/

	//Unload fonts
	App->fonts->Unload(score);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->player->player.plunger_ram->body->ApplyForce({ 0, -10 }, { 0, 0 }, true);		//Gets the plunger to its ready postion.
	
	//Load fonts (high score & score)
	//if (App->physics->debug == false) //Temporal measure to debug. Switches between the pinball map and the objects.
	//{	
		//TEXTURES-----------------------------------------

		//Load background
		App->renderer->Blit(board.background_tex, 0, 0, &board.background);

		//Load ditto
		App->renderer->Blit(board.background_tex, 4, 60, &board.ditto);

		//Load expanded ditto
		//App->renderer->Blit(element->background_tex, 7, 70, &element->ditto_expanded);
		
		//Load voltorb
		App->renderer->Blit(board.background_tex, 210, 270, &board.voltorb_boost);
		App->renderer->Blit(board.background_tex, 238, 186, &board.voltorb_boost);
		App->renderer->Blit(board.background_tex, 154, 214, &board.voltorb_boost);

		
		if(is_bouncer_hit[0] == true)
		{
			//Load boosted voltorb
			App->renderer->Blit(board.background_tex, 212, 270, &board.voltorb_boosted);
		}
		
		if (is_bouncer_hit[1] == true)
		{
			//Load boosted voltorb
			App->renderer->Blit(board.background_tex, 240, 186, &board.voltorb_boosted);
		}

		if (is_bouncer_hit[2] == true)
		{
			//Load boosted voltorb
			App->renderer->Blit(board.background_tex, 156, 214, &board.voltorb_boosted);
		}

		//Load blocker
		App->renderer->Blit(board.background_tex, 263, 64, &board.blocker);

		if (is_triangle_hit[0] == true)
		{
			//Load left triangle when hit
			App->renderer->Blit(board.background_tex, 95, 588, &board.triangle_boosted_L);
		}
		
		if (is_triangle_hit[1] == true)
		{
			//Load right triangle when hit
			App->renderer->Blit(board.background_tex, 302, 588, &board.triangle_boosted_R);
		}

		//ANIMATIONS----------------------------------------

		//Little starmie animation
		App->renderer->Blit(board.background_tex, -3, 420, &(board.starmie1.GetCurrentFrame()));

		//Left dugtrios animation
		App->renderer->Blit(board.background_tex, -3, 472, &(board.dugtrio_L.GetCurrentFrame()));

		//Right dugtrios animation
		App->renderer->Blit(board.background_tex, 379, 472, &(board.dugtrio_R.GetCurrentFrame()));

		//Diglett at left side animation
		App->renderer->Blit(board.background_tex, 64, 490, &(board.diglett_Left_Side.GetCurrentFrame()));

		//Diglett at right side animation
		App->renderer->Blit(board.background_tex, 347, 490, &(board.diglett_Right_Side.GetCurrentFrame()));

		if (App->player->player.lastWasRight == true)
		{
			//Pikachu right animation
			App->renderer->Blit(board.background_tex, 385, 686, &(board.pikachu.GetCurrentFrame()));
		}

		else if (App->player->player.lastWasLeft == true)
		{
			//Pikachu left animation
			App->renderer->Blit(board.background_tex, 18, 686, &(board.pikachu.GetCurrentFrame()));
		}

		//Bellsprout animation
		App->renderer->Blit(board.background_tex, 294, 224, &(board.bellsprout.GetCurrentFrame()));

		//Starmie2 animation
		App->renderer->Blit(board.background_tex, 122, 293, &(board.starmie2.GetCurrentFrame()));

		//Mid screen animation
		App->renderer->Blit(board.mid_tex, 153, 465, &(board.mid_screen.GetCurrentFrame()));
	
		//FONTS------------------------------------------------

		//Blit fonts
		sprintf_s(player_score, 10, "%7d", App->player->score);
		App->fonts->BlitText(130, 625, score, player_score, 0.7f);

		sprintf_s(max_score, 10, "%7d", App->player->maxscore);
		App->fonts->BlitText(289, 15, score, max_score, 0.7f);


		int x;
		int	y;

		if (board.dynamicBody_List.getFirst() != NULL)
		{
			p2List_item<PhysBody*>* dynBody_iterator = board.dynamicBody_List.getFirst();

			while (dynBody_iterator != NULL)
			{
				if (dynBody_iterator->data->body->GetFixtureList()->GetShape()->GetType() == 0)		//Type 0 means the shape of the fixture is a circle.
				{
					dynBody_iterator->data->GetPosition(x, y);
					App->renderer->Blit(App->player->player.pokeball_tex, x, y, NULL, 1.0f, dynBody_iterator->data->GetRotation());
				}
				if (dynBody_iterator->data->body->GetFixtureList()->GetShape()->GetType() == 2)		//Type 2 means the shape of the fixture is a polygon.
				{
					if (dynBody_iterator->data == App->player->player.left_flipper)
					{
						dynBody_iterator->data->GetPosition(x, y);
						App->renderer->Blit(App->player->player.flipper_Left_tex, x, y - 1, NULL, 1.0f, dynBody_iterator->data->GetRotation(), 2, 2);	//The last two arguments are how far from the center of roation the origin of the texture is.
					}
					else if (dynBody_iterator->data == App->player->player.right_flipper)
					{
						dynBody_iterator->data->GetPosition(x, y);
						App->renderer->Blit(App->player->player.flipper_Right_tex, x, y, NULL, 1.0f, dynBody_iterator->data->GetRotation(), -4, 3);		//The last two arguments are how far from the center of roation the origin of the texture is.
					}
				}
				if (dynBody_iterator->data->body->GetFixtureList()->GetShape()->GetType() == 3)		//Type 3 means the shape of the fixture is a chain.
				{
					dynBody_iterator->data->GetPosition(x, y);
					App->renderer->Blit(board.voltorb_tex, x, y, NULL, 1.0f, dynBody_iterator->data->GetRotation());
				}
				dynBody_iterator = dynBody_iterator->next;
			}
		}

		return UPDATE_CONTINUE;
	//}
}

bool ModuleSceneIntro::InitializeBoard()
{
	board.debugMode = true;
	return true;
}

bool ModuleSceneIntro::AddAnimationPushbacks()
{
	board.starmie1.PushBack({ 437, 2121, 54, 56 });
	board.starmie1.PushBack({ 493, 2121, 54, 56 });
	board.starmie1.loop = true;
	board.starmie1.speed = 0.1;

	board.dugtrio_L.PushBack({ 370, 2020, 67, 87 });
	board.dugtrio_L.PushBack({ 440, 2020, 67, 87 });
	board.dugtrio_L.PushBack({ 510, 2020, 67, 87 });
	board.dugtrio_L.PushBack({ 580, 2020, 67, 87 });
	board.dugtrio_L.loop = true;
	board.dugtrio_L.speed = 0.08;

	board.dugtrio_R.PushBack({ 903, 2020, 69, 87 });
	board.dugtrio_R.PushBack({ 973, 2020, 69, 87 });
	board.dugtrio_R.PushBack({ 1043, 2020, 69, 87 });
	board.dugtrio_R.PushBack({ 1113, 2020, 69, 87 });
	board.dugtrio_R.loop = true;
	board.dugtrio_R.speed = 0.08;

	board.diglett_Left_Side.PushBack({ 552, 2115, 35, 42 });
	board.diglett_Left_Side.PushBack({ 588, 2115, 31, 42 });
	board.diglett_Left_Side.PushBack({ 622, 2115, 25, 42 });
	board.diglett_Left_Side.loop = true;
	board.diglett_Left_Side.speed = 0.06;


	board.diglett_Right_Side.PushBack({ 938, 2115, 37, 42 });
	board.diglett_Right_Side.PushBack({ 902, 2115, 37, 43 });
	board.diglett_Right_Side.PushBack({ 973, 2115, 37, 42 });
	board.diglett_Right_Side.loop = true;
	board.diglett_Right_Side.speed = 0.06;

	board.pikachu.PushBack({ 477, 2345, 48, 46 });
	board.pikachu.PushBack({ 529, 2345, 42, 46 });
	board.pikachu.loop = true;
	board.pikachu.speed = 0.03;

	board.bellsprout.PushBack({ 861, 1684, 83, 104 });
	board.bellsprout.PushBack({ 942, 1687, 83, 104 });
	board.bellsprout.PushBack({ 1023, 1687, 83, 104 });
	board.bellsprout.PushBack({ 1107, 1690, 83, 104 });
	board.bellsprout.loop = true;
	board.bellsprout.speed = 0.03;

	board.starmie2.PushBack({ 575, 1833, 62, 64 });
	board.starmie2.PushBack({ 575, 1833, 62, 64 });
	board.starmie2.PushBack({ 575, 1833, 62, 64 });
	board.starmie2.PushBack({ 640, 1835, 62, 64 });
	board.starmie2.PushBack({ 640, 1835, 62, 64 });
	board.starmie2.PushBack({ 640, 1835, 62, 64 });
	board.starmie2.loop = true;
	board.starmie2.speed = 0.03;

	board.mid_screen.PushBack({ 1544, 14, 137, 92 });
	board.mid_screen.PushBack({ 1732, 14, 137, 92 });
	board.mid_screen.PushBack({ 1824, 14, 137, 92 });
	board.mid_screen.PushBack({ 1964, 14, 137, 92 });
	board.mid_screen.PushBack({ 2104, 14, 137, 92 });
	board.mid_screen.PushBack({ 2244, 14, 137, 92 });
	board.mid_screen.PushBack({ 2384, 14, 137, 92 });
	board.mid_screen.PushBack({ 284, 109, 137, 90 });
	board.mid_screen.PushBack({ 424, 109, 137, 90 });
	board.mid_screen.PushBack({ 564, 109, 137, 90 });
	board.mid_screen.PushBack({ 704, 109, 137, 90 });
	board.mid_screen.PushBack({ 844, 109, 137, 90 });
	board.mid_screen.PushBack({ 984, 109, 137, 90 });
	board.mid_screen.PushBack({ 1124, 109, 137, 90 });
	board.mid_screen.PushBack({ 1264, 109, 137, 90 });
	board.mid_screen.PushBack({ 1404, 109, 137, 90 });
	board.mid_screen.PushBack({ 1544, 109, 137, 90 });
	board.mid_screen.PushBack({ 1684, 109, 137, 90 });
	board.mid_screen.loop = true;
	board.mid_screen.speed = 0.009;

	return true;
}

//New This
bool ModuleSceneIntro::SetAnimationRectPosition()
{
	board.background.x = 1100.4;
	board.background.y = 11.2;
	board.background.w = 539;
	board.background.h = 780;

	board.ditto.x = 463.4;
	board.ditto.y = 1664.6;
	board.ditto.w = 74.2;
	board.ditto.h = 127.4;

	board.ditto_expanded.x = 557.2;
	board.ditto_expanded.y = 872.2;
	board.ditto_expanded.w = 110.6;
	board.ditto_expanded.h = 191.8;

	board.voltorb_boost.x = 726.6;
	board.voltorb_boost.y = 1639.4;
	board.voltorb_boost.w = 47.6;
	board.voltorb_boost.h = 49;

	board.voltorb_boosted.x = 779;
	board.voltorb_boosted.y = 1639.4;
	board.voltorb_boosted.w = 46;
	board.voltorb_boosted.h = 44;

	board.blocker.x = 820.4;
	board.blocker.y = 862.4;
	board.blocker.w = 148.4;
	board.blocker.h = 140;

	board.triangle_boosted_L.x = 649.6;
	board.triangle_boosted_L.y = 2242.8;
	board.triangle_boosted_L.w = 47;
	board.triangle_boosted_L.h = 79.8;

	board.triangle_boosted_R.x = 903;
	board.triangle_boosted_R.y = 2242.8;
	board.triangle_boosted_R.w = 46.2;
	board.triangle_boosted_R.h = 82.6;

	return true;
}