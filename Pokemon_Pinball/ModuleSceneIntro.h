#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

class PhysBody;
class ModuleSceneIntro;

struct PinballBoardElements		//Learn how to conver it into a class without recieving a nullptr at execution start.
{
	bool		debugMode;					//Bool for the debug mode, unused at the moment.
	
	//--------------------------PhysBodies declarations--------------------------
	PhysBody*	background_exterior;		//Static Chain that stablishes the exerior background of the Pinball Machine's Board
	PhysBody*	background_interior;		//Static Chain that stablishes the interior background of the Pinbal Machine' Board 
	PhysBody*	left_flipper;				//Polygon that represents the left flipper.
	PhysBody*	left_anchor;				//Static circle to which the left flipper attaches to with a revolution joint.
	PhysBody*	right_flipper;				//Polygon that represents the right flipper.
	PhysBody*	right_anchor;				//Static circle to which the right flipper attaches to with a revolution joint.
	PhysBody*	stopper;					//Static chain that stablishes the stopper between the flippers.
	PhysBody*	left_kicker;				//Static Chain that stablishes the left bumper triangle (kicker). It's restitution value (0 (no bounce) - 1 (bounces back with the same force)) needs to be modified to make it bounce the ball back.
	PhysBody*	right_kicker;				//Static Chain that stablishes the right bumper triangle (kicker). It's restitution value (0 (no bounce) - 1 (bounces back with the same force)) needs to be modified to make it bounce the ball back.
	PhysBody*	left_flipper_base;			//Static Chain that stablishes the left canal just below the left triangle.
	PhysBody*	right_flipper_base;			//Static Chain that stablishes the right canal just below the right triangle.
	PhysBody*	topLeft_Wall;				//Static Chain that stablishes the blue top-left wall just above the left Digglet.
	PhysBody*	starmie_Wall;				//Static Chain that stablihes the wall with the starmie on top. (Revise name)
	PhysBody*	centerLeft_miniWall;		//Static Chain that stablishes the center left wall just above the voltorbs.
	PhysBody*	centerRight_miniWall;		//Static Chain that stablishes the center right wall just above the voltorbs.
	PhysBody*	bellsprout_Wall;			//Static Chain that stablishes the wall bellsprout is at. (Revise name)
	PhysBody*	spinner;					//Sensor that detects if the ball has passed through the spinner at the right of the bellsprout wall.

	//--------------------------Animation declarations--------------------------
	//Textures
	SDL_Texture*	background_tex;
	SDL_Texture*	mid_tex;

	//Rects
	SDL_Rect	background;
	SDL_Rect	ditto;
	SDL_Rect	ditto_expanded;
	SDL_Rect	voltorb_boost;
	SDL_Rect	blocker;
	SDL_Rect	triangle_boosted_L;
	SDL_Rect	triangle_boosted_R;
	SDL_Rect	flipper_L;
	SDL_Rect	flipper_R;

	//Animations
	Animation	dugtrio_R;
	Animation	dugtrio_L;
	Animation	diglett_Right_Side;
	Animation	diglett_Left_Side;
	Animation	starmie1;
	Animation	pikachu;
	Animation	bellsprout;
	Animation	starmie2;

	Animation	mid_screen;


};

struct PinballBoardBodies		//Learn how to implement it with private elements instead of public.
{
	//------------------------------ Chains ------------------------------
	int backgroundExterior[124] = {			//Coordinates of the points that conform the Exterior background chain. 
		351, 557,		//Maybe need to change the 351 to 357
		351, 169,
		348, 153,
		346, 145,
		343, 137,
		341, 131,
		338, 122,
		334, 113,
		331, 107,
		326, 98,
		321, 90,
		315, 82,
		309, 74,
		292, 57,
		273, 43,
		262, 36,
		246, 29,
		236, 25,
		226, 22,
		214, 19,
		201, 17,
		142, 17,
		130, 20,
		118, 23,
		106, 27,
		97, 31,
		84, 38,
		72, 47,
		62, 56,
		56, 65,
		53, 65,
		53, 28,
		45, 18,
		41, 16,
		31, 16,
		27, 19,
		19, 28,
		19, 201,
		21, 222,
		23, 236,
		25, 249,
		27, 261,
		31, 273,
		35, 288,
		39, 302,
		43, 312,
		51, 324,
		57, 333,
		57, 337,
		46, 350,
		46, 385,
		36, 388,
		27, 392,
		20, 400,
		14, 413,
		14, 492,
		45, 512,
		113, 557,
		0, 557,
		0, 0,
		385, 0,
		385, 557
	};
};

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
	p2List<PhysBody*> body_list;
	
	PinballBoardElements board;			//As it is a class, it needs to be a pointer to be accessed from outside.
	PinballBoardBodies* body;
};


