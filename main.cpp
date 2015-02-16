#include "ecs.h"
#include <iostream>

struct pos : ecs::Component
{
	double x, y;
};

struct dir : ecs::Component
{
	double x, y;
};

struct movement : ecs::System
{
	movement(ecs::World& w) : ecs::System(w) {}

	virtual ecs::SystemType GetType() const override { return ecs::SystemType::Update; }

	virtual void Update(double deltaTime) override
	{
		for (auto& e : m_world.GetEntitiesWith<pos, dir>())
		{
			auto p = m_world.GetComponent<pos>(e);
			auto d = m_world.GetComponent<dir>(e);

			p->x += d->x * deltaTime;
			p->y += d->y * deltaTime;
		}
	};
};

void main()
{
	ecs::World world;

	// create an empty entity
	auto e1 = world.CreateEntity();

	// add a couple of components to that entity
	auto p = world.AddComponent<pos>(e1);
	auto d = world.AddComponent<dir>(e1);

	// initialize the components
	p->x = 2.0;
	p->y = 2.5;

	d->x = 4.3;
	d->y = 0.8;

	// add a new system to the world
	world.AddSystem<movement>();
	
	// update the world
	world.Update(1.0 / 60.0);
	std::cout << "pos: " << p->x << ", " << p->y << std::endl;
	world.Update(1.0 / 60.0);
	std::cout << "pos: " << p->x << ", " << p->y << std::endl;
	world.Update(1.0 / 60.0);
	std::cout << "pos: " << p->x << ", " << p->y << std::endl;

	// remove the movement system
	world.RemoveSystem<movement>();

	// update the world - the position of the entity should not change beyond this point
	world.Update(1.0 / 60.0);
	std::cout << "pos: " << p->x << ", " << p->y << std::endl;
	world.Update(1.0 / 60.0);
	std::cout << "pos: " << p->x << ", " << p->y << std::endl;
	world.Update(1.0 / 60.0);
	std::cout << "pos: " << p->x << ", " << p->y << std::endl;

	int a = 5;

}