#pragma once
#include "Component.h"
#include <vector>
#include <map>

namespace ecs
{
	struct System;

	class World
	{
	public:
		World();
		~World();

		// Explicitly disallow copying this class
		World(const World& copy) = delete;

	public:
		void Update(double deltaTime);
		void Render();

	public:
		const Entity::Id CreateEntity();
		void DeleteEntity(const Entity::Id& entity);

	public:
		template <typename C = Component>
		C* AddComponent(const Entity::Id& entity);

		template <typename C = Component>
		bool HasComponent(const Entity::Id& entity) const;

		template <typename C = Component>
		C* GetComponent(const Entity::Id& entity);

		template <typename C = Component>
		bool RemoveComponent(const Entity::Id& entity);

		template<typename A>
		const std::vector<Entity::Id> GetEntitiesWith();

		template<typename A, typename B>
		const std::vector<Entity::Id> GetEntitiesWith();

	public:
		template<typename S>
		S* AddSystem();

		template<typename S>
		bool HasSystem() const;

		template<typename S>
		S* GetSystem();

		template<typename S>
		bool RemoveSystem();

	private:
		typedef std::vector<Component*> ComponentArray;
		std::map<Entity::Id, ComponentArray> m_entities;

		std::vector<System*> m_systems;

	private:
		Entity::Id m_entityId;
	};
}

// Template functions implementations
#include "World.inl"
