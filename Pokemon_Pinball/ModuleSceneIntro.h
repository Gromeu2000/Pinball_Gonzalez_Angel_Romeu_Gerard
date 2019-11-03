#pragma once
#include "Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "p2List.h"

class PhysBody;
class ModuleSceneIntro;

struct PinballBoardElements		//Learn how to conver it into a class without recieving a nullptr at execution start.
{
	bool		debugMode;					//Bool for the debug mode, blits or unblits all images on screen.

	//--------------------------PhysBodies declarations--------------------------
	PhysBody*	background_exterior;		//Static Chain that stablishes the exerior background of the Pinball Machine's Board
	PhysBody*	background_interior;		//Static Chain that stablishes the interior background of the Pinbal Machine' Board 
	PhysBody*	stopper;					//Static chain that stablishes the stopper between the flippers.
	PhysBody*	left_kicker;				//Static Chain that stablishes the left bumper triangle (kicker). It's restitution value (0 (no bounce) - 1 (bounces back with the same force)) needs to be modified to make it bounce the ball back.
	PhysBody*	right_kicker;				//Static Chain that stablishes the right bumper triangle (kicker). It's restitution value (0 (no bounce) - 1 (bounces back with the same force)) needs to be modified to make it bounce the ball back.
	PhysBody*	left_arm;					//Static Chain that stablishes the left arm just below the left triangle.
	PhysBody*	right_arm;					//Static Chain that stablishes the right arm just below the right triangle.
	PhysBody*	topLeft_wall;				//Static Chain that stablishes the blue top-left wall just above the left Digglet.
	PhysBody*	starmie_wall;				//Static Chain that stablihes the wall with the starmie on top. (Revise name)
	PhysBody*	centerLeft_miniwall;		//Static Chain that stablishes the center left wall just above the voltorbs.
	PhysBody*	centerRight_miniwall;		//Static Chain that stablishes the center right wall just above the voltorbs.
	PhysBody*	bellsprout_wall;			//Static Chain that stablishes the wall bellsprout is at. (Revise name)
	PhysBody*	ball_below_flippers;		//Static Chain that stablishes the little ball below and in between the flippers.
	PhysBody*	spinner;					//Sensor that detects if the ball has passed through the spinner at the right of the bellsprout wall.
	
	// Bouncers
	PhysBody* bouncers[3];					//Static circles that stablish the 3 voltorb bouncers. 
	PhysBody* triangles[2];					//Static chains that stablish the 2 kicker triangles.
	PhysBody* left_diglett_bouncer;			//Static circle that will bump the ball away when the left Diglett is hit.
	PhysBody* right_diglett_bouncer;		//Static circle that will bump the ball away when the right Diglett is hit.

	p2List<PhysBody*> dynamicBody_List;		//List of dynamic Bodies, used to process all objects and assign an object its correspondent texture.
	p2List<PhysBody*> staticBody_List;		//List of static Bodies, used to process all objects and assign an object its correspondent texture.

	//--------------------------Animation declarations--------------------------
	//Textures
	SDL_Texture*	background_tex;
	SDL_Texture*	mid_tex;
	SDL_Texture*	voltorb_tex;

	//Rects
	SDL_Rect	background;
	SDL_Rect	ditto;
	SDL_Rect	ditto_expanded;
	SDL_Rect	voltorb_boost;
	SDL_Rect	voltorb_boosted;
	SDL_Rect	blocker;
	SDL_Rect	triangle_boosted_L;
	SDL_Rect	triangle_boosted_R;
	SDL_Rect	lights;
	SDL_Rect	again;

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

	//--------------------------Sensor Declarations--------------------------
	PhysBody* dying_sensor;
	PhysBody* left_kicker_sensor;
	PhysBody* right_kicker_sensor;
	PhysBody* ball_catcher;
	PhysBody* bellsprout_S;
	PhysBody* starmie_S;
	PhysBody* ditto_hole_sensor;
	PhysBody* left_wall_sensor;
	PhysBody* right_wall_sensor;
	PhysBody* light_sensor[4];
	PhysBody* toplight_sensor[3];
	PhysBody* voltorb_sensor[3];
	PhysBody* triangle_sensors[2];
	PhysBody* diglett_sensors[2];

	bool is_bouncer_hit[3] = { false, false, false };
	bool is_triangle_hit[2] = { false, false };
	bool is_light_sensor_hit[4] = { false, false, false, false };
	bool is_top_light_sensor_hit[3] = { false, false, false };

	//--------------------------Shape Declarations--------------------------
	//Tweak until it feels right.
	int backgroundExterior[132] = {
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
	79, 82,
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
	65, 495,
	85, 495, //Diglett start
	89, 499,
	91, 505,
	91, 518,
	89, 523,
	85, 526, //Diglett end
	65, 526,
	65, 535,
	54, 535,
	42, 540,
	30, 548,
	20, 561,
	12, 577,
	12, 684, //Left canal
	21, 692,
	62, 717,
	153, 780,
	-1, 780,
	-1, -1,
	539, -1,
	539, 780
	};

	int backgroundInterior[138] = {
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
	110, 117, //Tip of curve
	109, 120,
	112, 119,
	135, 104,
	164, 89,
	189, 81,
	202, 77,
	228, 73,
	256, 74,
	280, 78,
	302, 86,
	325, 96,
	347, 110,
	352, 114,
	355, 118,
	366, 127,
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
	384, 495, //Diglett start
	363, 495,
	360, 498,
	357, 504,
	357, 518,
	360, 523, 
	364, 526, //Diglett end
	384, 526,
	384, 535,
	395, 535,
	406, 539,
	415, 545,
	425, 556,
	433, 573,
	433, 677,
	428, 692,
	389, 717,
	296, 780
	};

	int left_Kicker[6] = {
	104, 585,
	140, 650,
	104, 625
	};

	int right_Kicker[6] = {
	345, 585,
	309, 650,
	345, 625
	};

	int left_Arm[16] = {
	62, 645,
	62, 586,
	60, 584,
	57, 586,
	57, 660,
	136, 705, //Final part 
	140, 700,
	62, 653
	};

	int right_Arm[16] = {
	386, 645,
	386, 586,
	387, 584,
	391, 586,
	391, 660,
	316, 704,
	310, 699,	//Final Part		
	386, 653
	};

	int topLeft_Wall[36] = {
	110, 423,
	126, 407,
	126, 404,
	99, 366,
	89, 339,
	80, 305,
	78, 276,
	80, 257,	//Topmost part of the wall
	82, 242,
	79, 240,
	78, 242,
	73, 257,
	71, 270,
	71, 317,
	75, 342,
	81, 369,
	89, 389,
	99, 412 
	};

	int starmie_Wall[38] = {
	157, 361,
	170, 361,
	185, 346,
	185, 333,
	179, 319,
	153, 293,
	141, 278,
	130, 266, //Interior side
	126, 249,
	126, 229,
	132, 211, //Topmost part of starmie wall
	138, 185,
	130, 205, //Left part of the wall
	126, 219,
	123, 229,
	123, 300,
	129, 318,
	135, 335,
	147, 353
	};

	int centerLeft_MiniWall[16] = {
	200, 136,
	196, 133,
	192, 134,
	187, 139,
	187, 176,
	189, 179,
	194, 179,
	200, 174
	};

	int centerRight_MiniWall[18] = {
	256, 120,
	260, 121,
	261, 127,
	261, 155,
	259, 157,
	252, 157,
	249, 154,
	249, 125,
	251, 120
	};

	int bellsprout_Wall[86] = {
	315, 144,		//Top part of the wall
	315, 235,
	312, 258,
	311, 277,
	302, 297,
	296, 315,
	294, 328,
	301, 334,
	304, 339,
	307, 339,
	313, 327,
	319, 312,
	322, 297,
	322, 267,
	322, 252,
	332, 269,		//Bellsprout
	357, 269,
	369, 243,
	369, 298,
	367, 311,
	365, 323,
	362, 337,
	356, 353,
	351, 366,
	345, 377,
	334, 391,
	324, 403,		//Number wall
	323, 407,
	337, 420,
	342, 418,
	352, 402,
	359, 386,
	365, 369,
	370, 355,
	374, 340,
	378, 318,
	378, 234,
	373, 219,
	367, 202,
	359, 188,
	351, 177,
	338, 162,
	324, 150 //Top part of the wall
	};

	/*int ball_Below_Flippers[14] = {
	218, 778,
	218, 772,
	221, 770,
	224, 769,
	227, 770,
	230, 772,
	230, 778
	};*/

	//Flippers
	int Left_Flipper[26] = {
	0, 20,
	0, 6,
	5, 0,
	22, 0,
	45, 3,
	64, 6,
	67, 9,
	67, 14,
	64, 17,
	50, 20,
	36, 22,
	22, 25,
	8, 25
	};

	int Left_Flipper_Long[28] = {
	0, 20,
	0, 6,
	5, 0,
	19, 0,
	31, 9,
	42, 17,
	53, 25,
	64, 37,
	64, 42,
	61, 45,
	53, 45,
	37, 37,
	22, 31,
	2, 23
	};

	int Right_Flipper[26] = {
	68, 20,
	59, 25,
	45, 25,
	31, 23,
	17, 20,
	3, 17,
	0, 14,
	0, 9,
	3, 6,
	23, 3,
	45, 0,
	62, 0,
	68, 6
	};

	int Right_Flipper_Long[20] = {
	45, 0,
	59, 0,
	64, 6,
	64, 20,
	48, 28,
	37, 34,
	12, 45,
	3, 45,
	0, 42,
	0, 37
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

public:
	PinballBoardElements board;			//As it is a class, it needs to be a pointer to be accessed from outside.
	
	bool InitializeBoard();				//Initializes all elements of the board.
	bool SetAnimationRectPosition();	//Sets each rect's data members with their corresponding measures.
	bool AddAnimationPushbacks();		//Load animations
	bool AddShapes();					//Loads all shapes
	bool AddSensors();					//Loads all sensors

	//Score
	char player_score[10];
	char max_score[10];
	char prev_score[10];
	int score = -1;
};


