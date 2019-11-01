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
	PhysBody*	left_arm;					//Static Chain that stablishes the left arm just below the left triangle.
	PhysBody*	right_arm;					//Static Chain that stablishes the right arm just below the right triangle.
	PhysBody*	topLeft_Wall;				//Static Chain that stablishes the blue top-left wall just above the left Digglet.
	PhysBody*	starmie_Wall;				//Static Chain that stablihes the wall with the starmie on top. (Revise name)
	PhysBody*	centerLeft_miniWall;		//Static Chain that stablishes the center left wall just above the voltorbs.
	PhysBody*	centerRight_miniWall;		//Static Chain that stablishes the center right wall just above the voltorbs.
	PhysBody*	bellsprout_Wall;			//Static Chain that stablishes the wall bellsprout is at. (Revise name)
	PhysBody*	spinner;					//Sensor that detects if the ball has passed through the spinner at the right of the bellsprout wall.
	PhysBody*	ball;						//Dynamic circle that will act as the ball.

	p2List<PhysBody*> dynamicBody_List;
	p2List<PhysBody*> staticBody_List;

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

	//--------------------------Shape Declarations--------------------------
	//Tweak until it feels right.
	int backgroundExterior[114] = {
	493, 780,
	493, 235,
	487, 202,
	481, 182,
	472, 158,
	458, 132,
	438, 103,
	417, 81,
	392, 62,
	367, 46,
	340, 32,
	311, 22,
	280, 19,
	240, 16, 
	200, 19,
	180, 25,
	152, 33,
	130, 42,
	110, 54,
	100, 62,
	79, 82,  //Out of the initial step.
	73, 82,
	73, 44,
	68, 33,
	61, 25,
	49, 22,
	39, 25,
	30, 33,
	23, 42,
	23, 90,
	22, 283,
	25, 316,
	31, 352,
	37, 377,
	43, 398,
	51, 420,
	59, 437,
	67, 450,
	74, 460,
	80, 466,
	80, 472,
	74, 475,
	69, 481,
	65, 490,
	65, 536,
	57, 536,
	46, 541,
	35, 547,
	27, 555,
	20, 566,
	12, 579, //Left canal
	12, 686,
	158, 780,
	0, 780,
	0, 0,
	538, 0,
	538, 780
	};

	int backgroundInterior[116] = {
	447, 780,
	447, 239,
	438, 205,
	427, 175,
	414, 149,
	399, 128,
	387, 116,
	349, 89,
	324, 78,
	302, 71,
	269, 66,
	256, 64,
	244, 64,
	213, 66,
	192, 70,
	174, 76,
	152, 85,
	129, 100,
	108, 125, //Out of the initial step
	107, 128,
	112, 125,
	135, 104,
	164, 89,
	189, 81,
	202, 77,
	228, 73,
	256, 74,
	280, 78,
	302, 86,
	320, 96,
	342, 110,
	362, 127,
	381, 146,
	399, 169,
	415, 201,
	426, 236,
	427, 284,
	428, 307,
	426, 335,
	421, 365,
	409, 395,
	395, 424,
	385, 442,
	376, 456,
	369, 465,
	369, 469,
	377, 474,
	382, 480,
	384, 487,
	384, 536,
	397, 536,
	410, 542,
	422, 551,
	429, 562,
	435, 579, //Right Canal
	435, 686,
	388, 715,
	290, 780
	};

	int left_Kicker[6] = {
	104, 585,
	140, 655,
	104, 630
	};

	int right_Kicker[6] = {
	345, 585,
	309, 655,
	345, 630
	};

	int left_Arm[16] = {
	62, 645,
	62, 586,
	60, 584,
	57, 586,
	57, 660,
	154, 724,
	155, 715,
	62, 653
	};

	int right_Arm[16] = {
	386, 645,
	386, 586,
	387, 584,
	391, 586,
	391, 660,
	296, 724,
	296, 715,
	386, 653
	};
};

//struct PinballBoardBodies		//Learn how to implement it with private elements instead of public.
//{
//	
//};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

private:

public:
	PinballBoardElements board;			//As it is a class, it needs to be a pointer to be accessed from outside.
	//PinballBoardBodies* body;
	
	void SetAnimationRectPosition();	//Sets each rect's data members with their corresponding measures.
	void AddAnimationPushbacks();		//Load animations
	void InitializeBoard();
};


