#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"		//Temporarily added due to debug key
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
				mouse_position.x = PIXELS_TO_METERS(App->input->GetMouseX());	//Gets the position of the mouse in the X axis.
				mouse_position.y = PIXELS_TO_METERS(App->input->GetMouseY());	//Gets the position of the mouse in the Y axis. 

				if (fixture->TestPoint(mouse_position))							//TestPoint() checks if a given position is inside a given object. Used to check if the point where the mouse was when it was clicked was inside the object being iterated.
				{
					clicked = true;													//Sets to true the bool that checks if an object has been clicked on.
					clickedObj = fixture->GetBody();							//Sets the pointer to b2Body of the object currently being iterated to clicked. Sets the object clicked.
					//clickedObj = body; //Also works.
				}
				
				//MouseJoint Method II
				/*mouse_position.x = PIXELS_TO_METERS(App->input->GetMouseX());				//Gets the position of the mouse in the X axis.
				mouse_position.y = PIXELS_TO_METERS(App->input->GetMouseY());				//Gets the position of the mouse in the Y axis.

				clickedObject = (PhysBody*)body->GetUserData();								//Gets the data memebers of a given object and fills the data members of foundBody with them. GetUserData() is used to store application data.

				if (clickedObject->Contains(mouse_position.x, mouse_position.y) == true)	//If the bool Contains() returns false, then it means that there is no object being clicked. (see above how TestPoint(mouse_position.x, mouse_position.y) is used to check whether or not the position passed as an argument is inside a given object.
				{
					clickedObject = (PhysBody*)body;													//Sets foundBody pointer to NULL.
				}*/
			}
		}
	}

	//-----------------------------------------Mouse Joint Method-----------------------------------------
	//MouseJoint Method I
	if (clicked == true)															//If clicked is true.
	{
		b2MouseJointDef mouseJoint_def;												//Defines a mouse joint frame.

		mouseJoint_def.bodyA = ground;												//Sets the unmovable object anchor point of the joint. BodyA is the first attached object.
		mouseJoint_def.bodyB = clickedObj;											//Sets the moving object anchor point of the joint. BodyB is the second attached object.
		mouseJoint_def.target = mouse_position;										//Sets the position to where BodyB will move to. It sets where the clicked body will move towards.
		mouseJoint_def.dampingRatio = 0.5f;											//Sets the damping ratio of the joint.
		mouseJoint_def.frequencyHz = 2.0f;											//Sets the response speed of the joint. It sets how fast the clicked object responds to the joint.
		mouseJoint_def.maxForce = 100.0f * clickedObj->GetMass();					//Sets the maximum constrain force that can be exerted. Sets the limits of how much force will the joint exert on the clicked object.

		mouse_joint = (b2MouseJoint*)world->CreateJoint(&mouseJoint_def);			//Creates the actual joint with the data members of mouseJoint_def and sets its data members to mouse_joint.
	}

	if (mouse_joint && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)	//Checks if the player is still pressing the left mouse button.
	{
		mouse_joint->SetTarget({ PIXELS_TO_METERS(App->input->GetMouseX()), PIXELS_TO_METERS(App->input->GetMouseY()) });																			//Set Target updates the target's position, in this case the mouse's position.
		App->renderer->DrawLine((App->input->GetMouseX()), (App->input->GetMouseY()), METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), 255, 255, 255);	//Draws a line from the clicked object to the mouse. GetAnchorB() gets the current position of the clicked body.
	}

	if (mouse_joint && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)		//Checks if the player has released the left mouse button.
	{
		world->DestroyJoint(mouse_joint);											//Destroys the mouse_joint that was created in the world.
		mouse_joint = nullptr;														//Sets the pointer of the mouse_joint back to nullptr.
	}
	
	//MouseJoint Method II
	/*if (clickedObject != NULL)													//Does not work bc clicked object is null the moment the ball gets out of the mouse position. Does not actualize AnchorB position.
	{
		b2MouseJointDef mouseJoint_def;												//Defines a mouse joint frame.

		mouseJoint_def.bodyA		= ground;										//Sets the unmovable object anchor point of the joint. BodyA is the first attached object.
		mouseJoint_def.bodyB		= clickedObject->body;							//Sets the moving object anchor point of the joint. BodyB is the second attached object.
		mouseJoint_def.target		= mouse_position;								//Sets the position to where BodyB will move to. It sets where the clicked body will move towards.
		mouseJoint_def.dampingRatio = 0.5f;											//Sets the damping ratio of the joint.
		mouseJoint_def.frequencyHz	= 2.0f;											//Sets the response speed of the joint. It sets how fast the clicked object responds to the joint.
		mouseJoint_def.maxForce		= 100.0f * clickedObject->body->GetMass();		//Sets the maximum constrain force that can be exerted. Sets the limits of how much force will the joint exert on the clicked object.
		
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&mouseJoint_def);			//Creates the actual joint with the data members of mouseJoint_def and sets its data members to mouse_joint.
	}*/

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

PhysBody* ModulePhysics::CreateCircle(b2BodyType type, int x, int y, int radius, int restitution)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXELS_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = restitution;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(b2BodyType type, int x, int y, int width, int height, int restitution)
{
	b2BodyDef body;
	body.type = type;
	body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXELS_TO_METERS(width) * 0.5f, PIXELS_TO_METERS(height) * 0.5f);

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

PhysBody* ModulePhysics::CreateFlipper(b2BodyType type, int x, int y, int* points, int size, int restitution)
{
	b2BodyDef body;														//Declares the frame of the geometrical form.

	body.type = type;													//Defines the body as static. Will remain unaffected by exterior forces.

																		//Sets the type of frame to dynamic, which means that it can be affected by external forces.
	body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));		//Sets the position in the world where the shape will be placed at.

	b2Body* b = world->CreateBody(&body);								//Creates a body given a definition.

	b2PolygonShape box;													//Defines that the shape that will be assigned to the b2Body* b (from b2BodyDef) will be a chain.
	b2Vec2* p = new b2Vec2[size / 2];									//Creates the array of vectors that recieve the coordinate points and generates a new vector out of them.

	for (uint i = 0; i < size / 2; ++i)									//Runs a loop that fills the array of vectors with the given coordinate points.
	{
		p[i].x = PIXELS_TO_METERS(points[i * 2 + 0]);					//Assigns the first value of each pair of coordinates that are passed to the X axis / property of the vector.
		p[i].y = PIXELS_TO_METERS(points[i * 2 + 1]);					//Assigns the second value of each pair of coordinates that are passed to the Y axis / property of the vector.
	}

	box.Set(p, size / 2);												//Creates a loop and automatically adjusts connectivity. The vector array and the total count of vectors are passed as an argument.

																		//Fixture has a bool (isSensor) that if set to true, it identifies the body/shape as a sensor which detects contact but does not return a collision.
	b2FixtureDef fixture;												//Definition of the fixture. It has multiple elements: Shape(circle), Density(Weight), Friction(How it drags along surfaces) and Restitution(how much it bounces back after impact)
	fixture.shape = &box;												//Defines which shape will the fixture have. Assigns a shape, in this case a chain.
	fixture.density = 1.0f;
	fixture.restitution = restitution;									//Defines the restitution value of a body. It normally ranges from 0 (no bounce) to 1 (bounces back to the same position)
	
	b->CreateFixture(&fixture);											//This method creates a fixture and adds it to a body. Used to assing parameters like denisity, friction, restitution...

	//delete p;															//As the information that the vector array had has been passed to the CreateLoop() method and the fixture has been created, the vector is not needed anymore and is deleted.

	PhysBody* pbody = new PhysBody();									//Creates a pointer to the PhysBody class, which keeps track of the body's position and rotation. 
	pbody->body = b;													//Assigns the pointer to b2Body (*b) to pbody, which gives the phys body class access to the data members that b has. 
	b->SetUserData(pbody);												//The SetUserData() method is used to store application specific data. Needs to be given a pointer.
	pbody->width = pbody->height = 0;									//Sets the width and the height of the body to 0;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(b2BodyType type, int x, int y, int* points, int size, int restitution)
{
	b2BodyDef body;													//Declares the frame of the geometrical form.
	
	body.type = type;												//Defines the body as static. Will remain unaffected by exterior forces.

																	//Sets the type of frame to dynamic, which means that it can be affected by external forces.
	body.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));	//Sets the position in the world where the shape will be placed at.

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

void ModulePhysics::CreateFlippers(/*PhysBody* dynamicB, PhysBody* staticB*/)
{
	App->scene_intro->board.left_flipper = CreateRectangle(b2_dynamicBody, 153, 716, 64, 25, 0);
	App->scene_intro->board.left_anchor = App->physics->CreateCircle(b2_staticBody, 158, 722, 7, 0);

	b2RevoluteJointDef def_l;

	def_l.bodyA = App->scene_intro->board.left_flipper->body;
	def_l.bodyB = App->scene_intro->board.left_anchor->body;
	def_l.collideConnected = false;
	def_l.upperAngle = DEGTORAD * 25;
	def_l.lowerAngle = DEGTORAD * -25;
	def_l.enableLimit = true;
	def_l.localAnchorA.Set(PIXELS_TO_METERS(-12), PIXELS_TO_METERS(-6));
	App->physics->left_Anchor = (b2RevoluteJoint*)world->CreateJoint(&def_l);

	
	App->scene_intro->board.right_flipper = CreateRectangle(b2_dynamicBody, 300, 716, 64, 25, 0);
	App->scene_intro->board.right_anchor = App->physics->CreateCircle(b2_staticBody, 288, 722, 7, 0);
	
	b2RevoluteJointDef def_r;

	def_r.bodyA = App->scene_intro->board.right_flipper->body;
	def_r.bodyB = App->scene_intro->board.right_anchor->body;
	def_r.collideConnected = false;
	def_r.upperAngle = DEGTORAD * 25;
	def_r.lowerAngle = DEGTORAD * -25;
	def_r.enableLimit = true;
	def_r.localAnchorA.Set(PIXELS_TO_METERS(51), PIXELS_TO_METERS(12));
	App->physics->right_Anchor = (b2RevoluteJoint*)world->CreateJoint(&def_r);

}