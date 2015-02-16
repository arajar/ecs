#pragma once
#include "Identifier.h"

namespace ecs
{
	class World;

	enum class SystemType
	{
		Update,
		Render,
		None,
	};

	struct System
	{
		ID(System);

		System(World& world) : m_world(world), m_type(SystemType::None), m_priority(-1) {}

		virtual SystemType GetType() const { return m_type; }
		virtual int GetPriority() const { return m_priority; }

		virtual void Render() {};
		virtual void Update(double deltaTime) {};

	protected:
		World& m_world;

		SystemType m_type;
		int m_priority;
	};
}
