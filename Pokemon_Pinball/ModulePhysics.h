#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -5.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
	uint score;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener		//Inherits from b2ContactListener so ModulePhyisics can act as a ContactListener.
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(b2BodyType type, int x, int y, int radius, int restitution, int friction = 0);
	PhysBody* CreateCircleSensor(int x, int y, int radius, int score = 0);
	PhysBody* CreateRectangle(b2BodyType type, int x, int y, int width, int height, int restitution, float angle = 0.0f);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, float angle = 0.0f, int score = 0);
	PhysBody* CreateChain(b2BodyType type, int x, int y, int* points, int size, int restitution);	//Method that creates chains. Has a bool to make the chain static or dynamic.
	PhysBody* CreateFlipper(b2BodyType type, int x, int y, int* points, int size, int restitution); //Method that creates poligons with the given verexs.
	void CreateRevolutionJoint(PhysBody* dynamicBody, PhysBody* staticBody, int upperAngle, int lowerAngle, int offsetX, int offsetY, b2RevoluteJoint* revoluteJoint);
	void CreatePrismaticJoint(PhysBody* dynamicBody, PhysBody* staticBody, b2PrismaticJoint* prismaticJoint);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	bool debug;

	b2World* world = nullptr;		//Creates a pointer to a b2World

private:

	b2MouseJoint* mouse_joint;		//Creates a pointer to a MouseJoint
	b2Body* ground;					//Creates a pointer to a body. In this case this body will be used to set the anchor point in a mouseJoint.
	b2Body* clickedObject;				
	//PhysBody* clickedObject;		//Body that will get the pointer of a clicked object.

	b2RevoluteJoint* left_Anchor;	//For the rectangle flipper method
	b2RevoluteJoint* right_Anchor;	//For the rectangle flipper method

	b2Vec2 mouse_position;
	b2Vec2 object_position;
	bool clicked;

	friend class ModuleSceneIntro; //Gives the ModuleSceneIntro class access to private methods and variables from class ModulePhysics. (friend class) Temporal.
};