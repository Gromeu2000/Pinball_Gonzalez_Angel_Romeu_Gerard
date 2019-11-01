#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
#include "ModuleTextures.h"
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

	//Load Shapes:
	//----------------------------Static Shapes----------------------------
	//Static Chains:
	//Background Chains. All chains have their pivot at (0, 0) due to Ric's vector drawing program
	board.background_exterior = App->physics->CreateChain(0, 0, board.backgroundExterior, 114, true);
	board.background_interior = App->physics->CreateChain(0, 0, board.backgroundInterior, 116, true);
	board.left_kicker = App->physics->CreateChain(0, 0, board.left_Kicker, 6, true);
	board.right_kicker = App->physics->CreateChain(0, 0, board.right_Kicker, 6, true);
	board.left_arm = App->physics->CreateChain(0, 0, board.left_Arm, 16, true);
	board.right_arm = App->physics->CreateChain(0, 0, board.right_Arm, 16, true);

	//Load music
	App->audio->PlayMusic("audio/Songs/Main_Theme.ogg");

	//Load textures
	board.background_tex = App->textures->Load("sprites/Pokemon_Pinball_Board_Spritesheet.png");
	board.mid_tex = App->textures->Load("sprites/Pokemon_Pinball_Special_Sprites_Spritesheet.png");

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

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		board.ball = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 20);
		board.dynamicBody_List.add(board.ball);
	}

	
	if (App->physics->debug == false) //Temporal measure to debug. Switches between the pinball map and the objects.

	{	//TEXTURES-----------------------------------------

		//Load background
		App->renderer->Blit(board.background_tex, 0, 0, &board.background);

		//Load ditto
		App->renderer->Blit(board.background_tex, 4.2, 60.2, &board.ditto);

		//Load expanded ditto
		//App->renderer->Blit(element->background_tex, 7, 70, &element->ditto_expanded);

		//Load voltorb
		App->renderer->Blit(board.background_tex, 210, 270.2, &board.voltorb_boost);
		App->renderer->Blit(board.background_tex, 238, 186.2, &board.voltorb_boost);
		App->renderer->Blit(board.background_tex, 154, 214.2, &board.voltorb_boost);

		//Load blocker
		App->renderer->Blit(board.background_tex, 263.2, 64.4, &board.blocker);

		//Load left triangle when hit
		App->renderer->Blit(board.background_tex, 95.2, 588, &board.triangle_boosted_L);

		//Load right triangle when hit
		App->renderer->Blit(board.background_tex, 302.4, 588, &board.triangle_boosted_R);

		//ANIMATIONS----------------------------------------

		//Little starmie animation
		App->renderer->Blit(board.background_tex, -2.8, 420, &(board.starmie1.GetCurrentFrame()));

		//Left dugtrios animation
		App->renderer->Blit(board.background_tex, -2.8, 471.8, &(board.dugtrio_L.GetCurrentFrame()));

		//Right dugtrios animation
		App->renderer->Blit(board.background_tex, 379.4, 471.8, &(board.dugtrio_R.GetCurrentFrame()));

		//Diglett at left side animation
		App->renderer->Blit(board.background_tex, 64.4, 490, &(board.diglett_Left_Side.GetCurrentFrame()));

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
			App->renderer->Blit(board.background_tex, 18.2, 686, &(board.pikachu.GetCurrentFrame()));
		}

		//Bellsprout animation
		App->renderer->Blit(board.background_tex, 210, 160, &(board.bellsprout.GetCurrentFrame()));

		//Starmie2 animation
		App->renderer->Blit(board.background_tex, 87, 209, &(board.starmie2.GetCurrentFrame()));

		//Mid screen animation
		App->renderer->Blit(board.mid_tex, 109, 332, &(board.mid_screen.GetCurrentFrame()));

		
		return UPDATE_CONTINUE;
	}
}

void InitializeBoard()
{

}

void ModuleSceneIntro::AddAnimationPushbacks()
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

	board.bellsprout.PushBack({ 615, 1203, 59, 74 });
	board.bellsprout.PushBack({ 673, 1205, 59, 74 });
	board.bellsprout.PushBack({ 731, 1205, 59, 74 });
	board.bellsprout.PushBack({ 791, 1207, 59, 74 });
	board.bellsprout.loop = true;
	board.bellsprout.speed = 0.03;

	board.starmie2.PushBack({ 411, 1309, 44, 46 });
	board.starmie2.PushBack({ 411, 1309, 44, 46 });
	board.starmie2.PushBack({ 411, 1309, 44, 46 });
	board.starmie2.PushBack({ 457, 1311, 44, 48 });
	board.starmie2.PushBack({ 457, 1311, 44, 48 });
	board.starmie2.PushBack({ 457, 1311, 44, 48 });
	board.starmie2.loop = true;
	board.starmie2.speed = 0.03;

	board.mid_screen.PushBack({ 1103, 10, 98, 66 });
	board.mid_screen.PushBack({ 1203, 10, 98, 66 });
	board.mid_screen.PushBack({ 1303, 10, 98, 66 });
	board.mid_screen.PushBack({ 1403, 10, 98, 66 });
	board.mid_screen.PushBack({ 1503, 10, 98, 66 });
	board.mid_screen.PushBack({ 1603, 10, 98, 66 });
	board.mid_screen.PushBack({ 1703, 10, 98, 66 });
	board.mid_screen.PushBack({ 203, 78, 98, 64 });
	board.mid_screen.PushBack({ 303, 78, 98, 64 });
	board.mid_screen.PushBack({ 403, 78, 98, 64 });
	board.mid_screen.PushBack({ 503, 78, 98, 64 });
	board.mid_screen.PushBack({ 603, 78, 98, 64 });
	board.mid_screen.PushBack({ 703, 78, 98, 64 });
	board.mid_screen.PushBack({ 803, 78, 98, 64 });
	board.mid_screen.PushBack({ 903, 78, 98, 64 });
	board.mid_screen.PushBack({ 1003, 78, 98, 64 });
	board.mid_screen.PushBack({ 1103, 78, 98, 64 });
	board.mid_screen.PushBack({ 1203, 78, 98, 64 });
	board.mid_screen.loop = true;
	board.mid_screen.speed = 0.009;
}

//New This
void ModuleSceneIntro::SetAnimationRectPosition()
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
}