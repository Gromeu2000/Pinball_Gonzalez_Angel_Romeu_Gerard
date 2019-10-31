#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"


#include "Box2D/Box2D/Box2D.h"
#ifdef _DEBUG
#pragma comment(lib, "Box2D/libx86/Debug/Box2D.lib")
#else
#pragma comment(lib, "Box2D/libx86/Release/Box2D.lib")
#endif // DEBUG

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	//Declaring the Background Chains (Maybe move to ModuleIntroScene):
	
	//No need to create a b2Vec2 as the creation of it is already done in the CreateChain() method.
	//b2Vec2 exterior_Vec[62];				//This array is uninitialized. To initalize all the elements of the array to zero: b2Vec2 bckgndExteriorVec[62] = {};

	//Exterior Background chain.
	// Pivot 0, 0
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

	PhysBody * exterior_chain = CreateChain(0, 0, backgroundExterior, 124, true);
	
	/*for (int i = 0; backgroundExterior[i] != NULL; i++)
	{
		exterior_Vec[i].Set(PIXEL_TO_METERS(backgroundExterior[i * 2]), PIXEL_TO_METERS(backgroundExterior[i * 2 + 1]));
	}*/

	//b2BodyDef exteriorChain_Def;				//Defines the frame of a geometrical body/shape
	//exteriorChain_Def.type = b2_staticBody;		//Sets the type of the body definition to static. Static bodies cannot be affected by any forces.

	//exteriorChain_Def.position.Set(0, 0);		//Sets the initial position of the exterior chain.

	//b2Body * extChain_Body = world->CreateBody(&exteriorChain_Def);	//The  exterior_chain body definition is passed to the world object to create the body.

	//b2ChainShape exterior_chain;				//Creates the exterior chain. Chains have two-sided collisions and generate smooth collisions with information connectivity. Holds ALL the data required to construct a rigid body. In this case a chain.

	//exterior_chain.CreateLoop(exterior_Vec, 62);

	//Interior Background Chain 
	// Pivot 0, 0
	int backgroundInterior[80] = {
		321, 557,
		321, 168,
		314, 138,
		300, 110,
		290, 96,
		277, 81,
		256, 67,
		235, 57,
		206, 47,
		154, 47,
		125, 54,
		107, 62,
		89, 74,
		77, 84,
		80, 87,
		101, 71,
		128, 60,
		145, 56,
		182, 56,
		205, 62,
		225, 70,
		246, 83,
		279, 114,
		294, 142,
		303, 171,
		303, 203,
		297, 251,
		291, 276,
		282, 307,
		265, 335,
		272, 341,
		276, 349,
		276, 386,
		291, 390,
		302, 400,
		308, 414,
		308, 494,
		278, 512,
		242, 535,
		209, 557
	};

	PhysBody* interior_chain = CreateChain(0, 0, backgroundInterior, 80, true);

	

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 8, 3);

	return UPDATE_CONTINUE;
}


update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)		//Debug Key. Draws on screen al the shapes and allows to use a mouse joint.
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 0, 0, 0);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!

	delete world;

	return true;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXELS_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(width) * 0.5f, PIXELS_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef rectSensorBody;
	rectSensorBody.type = b2_staticBody;
	rectSensorBody.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	b2Body* b = world->CreateBody(&rectSensorBody);

	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(width) * 0.5f, PIXELS_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, bool isStatic) //Chapuza to be able to pass the function a bool that sets the type of the definition.
{
	b2BodyDef body;													//Declares the frame of the geometrical form.
	
	if (isStatic)
	{
		body.type = b2_staticBody;
	}
	else
	{
		body.type = b2_dynamicBody;
	}
																	//Sets the type of frame to dynamic, which means that it can be affected by external forces.
	body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));		//Sets the position in the world where the shape will be placed at.

	b2Body* b = world->CreateBody(&body);							//Creates a body given a definition.

	b2ChainShape shape;												//Defines that the shape that will be assigned to the b2Body* b (from b2BodyDef) will be a chain.
	b2Vec2* p = new b2Vec2[size / 2];								//Creates the array of vectors that recieve the coordinate points and generates a new vector out of them.

	for (uint i = 0; i < size / 2; ++i)								//Runs a loop that fills the array of vectors with the given coordinate points.
	{
		p[i].x = PIXELS_TO_METERS(points[i * 2 + 0]);				//Assigns the first value of each pair of coordinates that are passed to the X axis / property of the vector.
		p[i].y = PIXELS_TO_METERS(points[i * 2 + 1]);				//Assigns the second value of each pair of coordinates that are passed to the Y axis / property of the vector.
	}

	shape.CreateLoop(p, size / 2);									//Creates a loop and automatically adjusts connectivity. The vector array and the total count of vectors are passed as an argument.

																	//Fixture has a bool (isSensor) that if set to true, it identifies the body/shape as a sensor which detects contact but does not return a collision.
	b2FixtureDef fixture;											//Definition of the fixture. It has multiple elements: Shape(circle), Density(Weight), Friction(How it drags along surfaces) and Restitution(how much it bounces back after impact)
	fixture.shape = &shape;											//Defines which shape will the fixture have. Assigns a shape, in this case a chain.

	b->CreateFixture(&fixture);										//This method creates a fixture and adds it to a body. Used to assing parameters like denisity, friction, restitution...

	delete p;														//As the information that the vector array had has been passed to the CreateLoop() method and the fixture has been created, the vector is not needed anymore and is deleted.

	PhysBody* pbody = new PhysBody();								//Creates a pointer to the PhysBody class, which keeps track of the body's position and rotation. 
	pbody->body = b;												//Assigns the pointer to b2Body (*b) to pbody, which gives the phys body class access to the data members that b has. 
	b->SetUserData(pbody);											//The SetUserData() method is used to store application specific data. Needs to be given a pointer.
	pbody->width = pbody->height = 0;								//Sets the width and the height of the body to 0;

	return pbody;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if (physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXELS_TO_METERS(x1), PIXELS_TO_METERS(y1));
	input.p2.Set(PIXELS_TO_METERS(x2), PIXELS_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}