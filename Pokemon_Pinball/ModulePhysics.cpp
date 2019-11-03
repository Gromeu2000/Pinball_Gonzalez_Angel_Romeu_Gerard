#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"		//Temporarily added due to debug key
#include "ModulePlayer.h"
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
	world->SetContactListener(this);						//This makes ModulePhysics a contact listener, whithout this collisions are not detected.

	//Joint relevant variables
	b2BodyDef  body_def;					//b2BodyDef holds all the information needed to create a rigid body.
	ground = world->CreateBody(&body_def);  //CreateBody() creates a rigid body given a definition. Sets ground as a rigid body.
	
	//Chains were declared here before.

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 8, 3);

	for (b2Contact* contact = world->GetContactList(); contact; contact = contact->GetNext())
	{
		if (contact->GetFixtureA()->IsSensor() && contact->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

			if (pb1 && pb2 && pb1->listener)
			{
				pb1->listener->OnCollision(pb1, pb2);
			}
		}
	}

	return UPDATE_CONTINUE;
}


update_status ModulePhysics::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)		//Debug Key. Draws on screen al the shapes and allows to use a mouse joint.
	{
		debug = !debug;
	}
		
	if (!debug)
	{
		return UPDATE_CONTINUE;
	}
	
	clicked = false; //Sets the clicked bool to false and allows the object to continue its course once the left button has been released.
	
	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext())							//Loop that iterates all objects that are in world->GetBodyList().
	{
		for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())	//Loop that iterates all the fixtures of an object. In this case the object is passed by the loop.
		{
			switch (fixture->GetType())																//Switch that checks the type of the object being iterated.
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:																//If the fixture type of the object being passed is circle, then it runs.
				{
					b2CircleShape* shape = (b2CircleShape*)fixture->GetShape();																		//Gets the shape of the object being iterated and sets it as a b2PolygonShape
					b2Vec2 pos = fixture->GetBody()->GetPosition();																					//Gets the position vector of the object being passed.
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 0, 0);		//Draws the circle object being iterated on screen using the DrawCircle() method.
				}
				break;
	
				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:															//If the fixture type of the object being passed is a poligon, then it runs.
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)fixture->GetShape();			//Gets the shape of the object being iterated and sets it as a b2PolygonShape.
					int32 count = polygonShape->GetVertexCount();									//Gets the number of vertexs of the polygon object that is being iterated.
					b2Vec2 prev, v;																	//Declares two b2Vec2 that will be used to define the origin vector and the end vector.
					
					for (int32 i = 0; i < count; ++i)												//Revise all the draw loop and make comments.
					{
						v = body->GetWorldPoint(polygonShape->GetVertex(i));
						if (i > 0) 
						{
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
						}

						prev = v;
					}

					v = body->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)fixture->GetShape();
					b2Vec2 prev, v;
	
					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = body->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
						{
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						}

						prev = v;
					}
	
					v = body->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;
	
				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)fixture->GetShape();
					b2Vec2 v1, v2;
					
					v1 = body->GetWorldPoint(shape->m_vertex0);
					v1 = body->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			} //----------------------------------------Switch Ends Here------------------------------------

			//Mouse Joint Method: Checking if there is an object where the mouse is at.
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)		//Checks if the player has pressed the left button of the mouse, SDL_BUTTON_LEFT returns 1.
			{
				//MouseJoint Method I
				mouse_position.x = PIXEL_TO_METERS(App->input->GetMouseX());	//Gets the position of the mouse in the X axis.
				mouse_position.y = PIXEL_TO_METERS(App->input->GetMouseY());	//Gets the position of the mouse in the Y axis. 

				if (fixture->TestPoint(mouse_position))							//TestPoint() checks if a given position is inside a given object. Used to check if the point where the mouse was when it was clicked was inside the object being iterated.
				{
					clicked = true;													//Sets to true the bool that checks if an object has been clicked on.
					clickedObject = fixture->GetBody();							//Sets the pointer to b2Body of the object currently being iterated to clicked. Sets the object clicked.
					//clickedObj = body; //Also works.
				}
			}
		}
	}

	//-----------------------------------------Mouse Joint Method-----------------------------------------
	//MouseJoint Method I
	if (clicked == true)															//If clicked is true.
	{
		b2MouseJointDef mouseJoint_def;												//Defines a mouse joint frame.

		mouseJoint_def.bodyA = ground;												//Sets the unmovable object anchor point of the joint. BodyA is the first attached object.
		mouseJoint_def.bodyB = clickedObject;											//Sets the moving object anchor point of the joint. BodyB is the second attached object.
		mouseJoint_def.target = mouse_position;										//Sets the position to where BodyB will move to. It sets where the clicked body will move towards.
		mouseJoint_def.dampingRatio = 0.5f;											//Sets the damping ratio of the joint.
		mouseJoint_def.frequencyHz = 2.0f;											//Sets the response speed of the joint. It sets how fast the clicked object responds to the joint.
		mouseJoint_def.maxForce = 100.0f * clickedObject->GetMass();					//Sets the maximum constrain force that can be exerted. Sets the limits of how much force will the joint exert on the clicked object.

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&mouseJoint_def);			//Creates the actual joint with the data members of mouseJoint_def and sets its data members to mouse_joint.
	}

	if (mouse_joint && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)	//Checks if the player is still pressing the left mouse button.
	{
		mouse_joint->SetTarget({ PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) });																			//Set Target updates the target's position, in this case the mouse's position.
		App->renderer->DrawLine((App->input->GetMouseX()), (App->input->GetMouseY()), METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), 255, 255, 255);	//Draws a line from the clicked object to the mouse. GetAnchorB() gets the current position of the clicked body.
	}

	if (mouse_joint && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)		//Checks if the player has released the left mouse button.
	{
		world->DestroyJoint(mouse_joint);											//Destroys the mouse_joint that was created in the world.
		mouse_joint = nullptr;														//Sets the pointer of the mouse_joint back to nullptr.
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

PhysBody* ModulePhysics::CreateCircle(b2BodyType type, int x, int y, int radius, int restitution, int friction)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = restitution;
	fixture.friction = 1.0f;						//Implement this only on the ball, pass it as an argument.

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

//Creates a circle sensor. Sensors are always static and detect contact. They do not return a collision however.
PhysBody* ModulePhysics::CreateCircleSensor(int x, int y, int radius, int score)
{
	b2BodyDef circleSensorBody;
	circleSensorBody.type = b2_staticBody;
	circleSensorBody.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&circleSensorBody);

	b2CircleShape circle;
	circle.m_radius = PIXEL_TO_METERS(radius);

	b2FixtureDef fixture;
	fixture.shape = &circle;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;
	pbody->score = score;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(b2BodyType type, int x, int y, int width, int height, int restitution, float angle)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b->SetTransform(body.position, DEGTORAD * angle);		//Changes the angle that rect is created with.

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

//Creates a rectangle sensor. Sensors are always static and detect contact. They do not return a collision however.
PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, float angle, int score)
{
	b2BodyDef rectSensorBody;
	rectSensorBody.type = b2_staticBody;
	rectSensorBody.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&rectSensorBody);
	b->SetTransform(rectSensorBody.position, DEGTORAD * angle);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

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
	pbody->score = score;

	return pbody;
}

PhysBody* ModulePhysics::CreateFlipper(b2BodyType type, int x, int y, int* points, int size, int restitution)
{
	b2BodyDef body;													//Declares the frame of the geometrical form.

	body.type = type;												//Defines the body as static. Will remain unaffected by exterior forces.

																	//Sets the type of frame to dynamic, which means that it can be affected by external forces.
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));		//Sets the position in the world where the shape will be placed at.

	b2Body* b = world->CreateBody(&body);							//Creates a body given a definition.

	b2PolygonShape box;												//Defines that the shape that will be assigned to the b2Body* b (from b2BodyDef) will be a chain.
	b2Vec2* p = new b2Vec2[size / 2];								//Creates the array of vectors that recieve the coordinate points and generates a new vector out of them.

	for (uint i = 0; i < size / 2; ++i)								//Runs a loop that fills the array of vectors with the given coordinate points.
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);				//Assigns the first value of each pair of coordinates that are passed to the X axis / property of the vector.
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);				//Assigns the second value of each pair of coordinates that are passed to the Y axis / property of the vector.
	}

	box.Set(p, size / 2);											//Creates a loop and automatically adjusts connectivity. The vector array and the total count of vectors are passed as an argument.

																	//Fixture has a bool (isSensor) that if set to true, it identifies the body/shape as a sensor which detects contact but does not return a collision.
	b2FixtureDef fixture;											//Definition of the fixture. It has multiple elements: Shape(circle), Density(Weight), Friction(How it drags along surfaces) and Restitution(how much it bounces back after impact)
	fixture.shape = &box;											//Defines which shape will the fixture have. Assigns a shape, in this case a chain.
	fixture.density = 1.0f;
	fixture.restitution = restitution;								//Defines the restitution value of a body. It normally ranges from 0 (no bounce) to 1 (bounces back to the same position)
	
	b->CreateFixture(&fixture);										//This method creates a fixture and adds it to a body. Used to assing parameters like denisity, friction, restitution...

	delete p;														//As the information that the vector array had has been passed to the CreateLoop() method and the fixture has been created, the vector is not needed anymore and is deleted.

	PhysBody* pbody = new PhysBody();								//Creates a pointer to the PhysBody class, which keeps track of the body's position and rotation. 
	pbody->body = b;												//Assigns the pointer to b2Body (*b) to pbody, which gives the phys body class access to the data members that b has. 
	b->SetUserData(pbody);											//The SetUserData() method is used to store application specific data. Needs to be given a pointer.
	pbody->width = pbody->height = 0;								//Sets the width and the height of the body to 0;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(b2BodyType type, int x, int y, int* points, int size, int restitution)
{
	b2BodyDef body;													//Declares the frame of the geometrical form.
	
	body.type = type;												//Defines the body as static. Will remain unaffected by exterior forces.

																	//Sets the type of frame to dynamic, which means that it can be affected by external forces.
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));		//Sets the position in the world where the shape will be placed at.

	b2Body* b = world->CreateBody(&body);							//Creates a body given a definition.

	b2ChainShape shape;												//Defines that the shape that will be assigned to the b2Body* b (from b2BodyDef) will be a chain.
	b2Vec2* p = new b2Vec2[size / 2];								//Creates the array of vectors that recieve the coordinate points and generates a new vector out of them.

	for (uint i = 0; i < size / 2; ++i)								//Runs a loop that fills the array of vectors with the given coordinate points.
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);				//Assigns the first value of each pair of coordinates that are passed to the X axis / property of the vector.
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);				//Assigns the second value of each pair of coordinates that are passed to the Y axis / property of the vector.
	}

	shape.CreateLoop(p, size / 2);									//Creates a loop and automatically adjusts connectivity. The vector array and the total count of vectors are passed as an argument.

																	//Fixture has a bool (isSensor) that if set to true, it identifies the body/shape as a sensor which detects contact but does not return a collision.
	b2FixtureDef fixture;											//Definition of the fixture. It has multiple elements: Shape(circle), Density(Weight), Friction(How it drags along surfaces) and Restitution(how much it bounces back after impact)
	fixture.shape = &shape;											//Defines which shape will the fixture have. Assigns a shape, in this case a chain.
	fixture.restitution = restitution;								//Defines the restitution value of a body. It normally ranges from 0 (no bounce) to 1 (bounces back to the same position)

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
	PhysBody* physb_A = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physb_B = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if (physb_A && physb_A->listener != NULL)
	{
		physb_A->listener->OnCollision(physb_A, physb_B);
	}

	if (physb_B && physb_B->listener != NULL)
	{
		physb_B->listener->OnCollision(physb_B, physb_A);
	}

	LOG("COLLISION");
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
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

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

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{

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

//Method to create a revolution joint given 2 bodies, the different relevant variables of a revoution joint and a pointer to a revolutionJoint.
void ModulePhysics::CreateRevolutionJoint(PhysBody* dynamicBody, PhysBody* staticBody, int upperAngle, int lowerAngle, int offsetX, int offsetY, b2RevoluteJoint* revoluteJoint)
{
	b2RevoluteJointDef revJoint_def;

	revJoint_def.bodyA				= dynamicBody->body;								//Sets bodyA as the dynamic part of the joint (the PhysBody* passed as an argument is dynamic)
	revJoint_def.bodyB				= staticBody->body;									//Sets bodyB as the static part of the joint (the PhysBody* passed as an argument is static)
	revJoint_def.collideConnected	= false;											//Determines if the joined objects will collide with each other or not.
	revJoint_def.upperAngle			= DEGTORAD * upperAngle;							//Defines the maximum positive angle the dynamic body can reach (upwards).
	revJoint_def.lowerAngle			= DEGTORAD * lowerAngle;							//Defines the maxumun negative angle the dynamic body can reach (downwards).
	revJoint_def.enableLimit		= true;												//Enables the joint limits so .upperAngle and .lowerAngle have an effect.
	revJoint_def.localAnchorA.Set(PIXEL_TO_METERS(offsetX), PIXEL_TO_METERS(offsetY));	//Sets how far is the origin of the dynamic body from the center of the static body.
	
	revoluteJoint = (b2RevoluteJoint*)world->CreateJoint(&revJoint_def);				//Creates the joint on the world.
}

//Method to create a prismatic joint given 2 bodies and a pointer to a prismaticJoint
void ModulePhysics::CreatePrismaticJoint(PhysBody* dynamicBody, PhysBody* staticBody, b2PrismaticJoint* prismaticJoint)
{
	b2PrismaticJointDef prismJoint_def;

	prismJoint_def.bodyA = dynamicBody->body;		//Defines the dynamicBody of the joint. Checks the type of body from the passed argument. It could also be static or kinematic.
	prismJoint_def.bodyB = staticBody->body;		//Defines the staticBody of the joint. Checks the type of body from the passed argument. It could also be dynamic or kinematic.
	prismJoint_def.collideConnected = true;			//Defines whether or not the joined bodies will collide.
	prismJoint_def.localAnchorA.Set(0, 1);			//Sets the local anchor point relative to the position of body A.
	prismJoint_def.localAnchorB.Set(0, -1);			//Sets thelocal anchor point relative to the position of body B.
	prismJoint_def.localAxisA.Set(0, -1);			//Local translation of body A.
	prismJoint_def.enableLimit = true;				//Enables jointLimit so .lowerTranslation and .upperTranslation  have an effect.
	prismJoint_def.lowerTranslation = -0.02;		//Sets the lower translation limit, normally in meters.
	prismJoint_def.upperTranslation = 2.0;			//Sets the upper translation limit, normally in meters.
	
	prismaticJoint = (b2PrismaticJoint*)world->CreateJoint(&prismJoint_def);
}

