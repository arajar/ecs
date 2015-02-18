#include "ecs.h"
#include <iostream>

struct pos : ecs::component
{
	double x = 0.0, y = 0.0;
};

struct dir : ecs::component
{
	double x, y;
};

struct movement2 : ecs::system
{
	movement2(ecs::world& world) : ecs::system(world) {}
	void operator()(double delta) const
	{
		for (auto& e : world_.search<pos>())
		{
			auto p = world_.get<pos>(e);

			p->x += 0.1 * delta;
			p->y += 0.1 * delta;
		}
	}
};

struct movement : ecs::system
{
	movement(ecs::world& world) : ecs::system(world) {}

	void operator()(double delta) const
	{
		for (auto& e : world_.search<pos, dir>())
		{
			auto p = world_.get<pos>(e);
			auto d = world_.get<dir>(e);

			p->x += d->x * delta;
			p->y += d->y * delta;
		}
	}
};

void main()
{
	ecs::world world;

	auto e1 = world.createEntity();
	auto e2 = world.createEntity();

	auto p1 = world.add<pos>(e1);
	p1->x = 2.3;
	p1->y = 0.4;

	auto d1 = world.add<dir>(e1);
	d1->x = 1.0;
	d1->y = 1.1;

	auto p2 = world.add<pos>(e2);
	p2->x = 3.0;
	p2->y = 4.0;

	world.add<movement>();
	world.add<movement2>();

	for (unsigned i = 0; i < 1000; ++i)
	{
		world(1.0 / 60.0);
	}

	std::cout << "e1 x = " << p1->x << ", " << p1->y << std::endl;
	std::cout << "e2 x = " << p2->x << ", " << p2->y << std::endl;
	int a = 3;
}