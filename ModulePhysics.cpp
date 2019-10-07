#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"

// TODO 1: Include Box 2 header and library
#include "Box2D/Box2D/Box2D.h"
#ifdef _DEBUG
#pragma comment(lib, "Box2D/libx86/Debug/Box2D.lib")
#else
#pragma comment(lib, "Box2D/libx86/Release/Box2D.lib")
#endif // DEBUG

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it

	b2Vec2 gravity(0.0f, 10.0f); 
	world = new b2World(gravity);


	// TODO 4: Create a a big static circle as "ground"

	b2BodyDef circle1;
	circle1.type = b2_staticBody;
	circle1.position.Set(PIXELS_TO_METERS(SCREEN_WIDTH/2), PIXELS_TO_METERS(SCREEN_HEIGHT/2));
	b2Body* body = world->CreateBody(&circle1);

	b2CircleShape shape;
	shape.m_radius = PIXELS_TO_METERS(SCREEN_WIDTH/4);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	body->CreateFixture(&fixture);

	b2BodyDef box;
	box.type = b2_staticBody;
	box.position.Set(25, 50);
	b2Body* body_b = world->CreateBody(&box);

	b2PolygonShape shape_b;
	shape_b.SetAsBox(27, 15);

	b2FixtureDef fixture_b;
	fixture_b.shape = &shape_b;
	body_b->CreateFixture(&fixture_b);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)

	world->Step(1.0f / 60.0f, 8, 3);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

		b2BodyDef body;
		body.type = b2_dynamicBody;
		body.position.Set(PIXELS_TO_METERS(App->input->GetMouseX()), PIXELS_TO_METERS(App->input->GetMouseY()));
		b2Body* body2 = world->CreateBody(&body);

		b2CircleShape shape;
		shape.m_radius = rand() % 5 + 1;

		b2FixtureDef fixture;
		fixture.shape = &shape;
		body2->CreateFixture(&fixture);
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;
				case b2Shape::e_polygon:
				{
					b2PolygonShape* groundBox = (b2PolygonShape*)f->GetShape();
					int32 count = groundBox->GetVertexCount();
					b2Vec2 previous, a_vector;

					for (int32 i = 0; i < count; ++i)
					{
						a_vector = b->GetWorldPoint(groundBox->GetVertex(i));
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(previous.x), METERS_TO_PIXELS(previous.y), METERS_TO_PIXELS(a_vector.x), METERS_TO_PIXELS(a_vector.y), 255, 100, 100);
						previous = a_vector;
					}
				}
				break;
				// You will have to add more cases to draw boxes, edges, and polygons ...
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
