#pragma once
#include "ecs.h"
#include "World.h"

#include <algorithm>

namespace ecs
{
	template <typename C>
	C* World::AddComponent(const Entity::Id& entity)
	{
		if (!HasComponent<C>(entity))
		{
			C* comp = new C;
			m_entities[entity].push_back(comp);
			return comp;
		}

		return nullptr;
	}

	template <typename C>
	bool World::HasComponent(const Entity::Id& entity) const
	{
		auto it = m_entities.find(entity);
		if (it != m_entities.end())
		{
			auto comps = (*it).second;
			for (auto& co : comps)
			{
				if (dynamic_cast<C*>(co))
				{
					return true;
				}
			}
		}

		return false;
	}

	template <typename C>
	C* World::GetComponent(const Entity::Id& entity)
	{
		if (m_entities.find(entity) != m_entities.end())
		{
			auto comps = m_entities[entity];
			for (auto& c : comps)
			{
				auto co = dynamic_cast<C*>(c);
				if (co != nullptr)
				{
					return co;
				}
			}
		}

		return nullptr;
	}

	template <typename C>
	bool World::RemoveComponent(const Entity::Id& entity)
	{
		if (m_entities.find(entity) != m_entities.end())
		{
			auto& comps = m_entities[entity];
			for (auto& c : comps)
			{
				auto co = dynamic_cast<C*>(c);
				comps.erase(co);
				return true;
			}
		}

		return false;
	}

	template<typename A>
	const std::vector<Entity::Id> World::GetEntitiesWith()
	{
		std::vector<Entity::Id> entities;

		auto itBegin = m_entities.begin();
		auto itEnd = m_entities.end();

		for (itBegin; itBegin != itEnd; ++itBegin)
		{
			auto comp = (*itBegin).second;
			for (int i = 0; i < comp.size(); i++)
			{
				if (dynamic_cast<A*>(comp[i]))
				{
					entities.push_back((*itBegin).first);
				}
			}
		}

		return entities;
	}

	// TODO: Use a variadic template for this
	template<typename A, typename B>
	const std::vector<Entity::Id> World::GetEntitiesWith()
	{
		std::vector<Entity::Id> entities;

		auto itBegin = m_entities.begin();
		auto itEnd = m_entities.end();

		for (itBegin; itBegin != itEnd; ++itBegin)
		{
			auto comp = (*itBegin).second;
			Component* a = nullptr;
			Component* b = nullptr;

			for (int i = 0; i < comp.size(); i++)
			{
				if (!a)
				{
					a = dynamic_cast<A*>(comp[i]);
				}

				if (!b)
				{
					b = dynamic_cast<B*>(comp[i]);
				}

				if (a && b)
				{
					entities.push_back((*itBegin).first);
					break;
				}
			}
		}

		return entities;
	}

	template<typename S>
	S* World::AddSystem()
	{
		auto sort = [](const System* a, const System* b)
		{
			return a->GetPriority() < b->GetPriority();
		};

		S* syst = new S(*this);
		m_systems.push_back(syst);
		std::sort(m_systems.begin(), m_systems.end(), sort);

		return syst;
	}

	template<typename S>
	bool World::HasSystem() const
	{
		for (auto& system : m_systems)
		{
			if (dynamic_cast<S*>(system))
			{
				return true;
			}
		}

		return false;
	}

	template<typename S>
	S* World::GetSystem()
	{
		for (auto& system : m_systems)
		{
			auto s = dynamic_cast<S*>(system);
			if (s)
			{
				return s;
			}
		}

		return nullptr;
	}

	template<typename S>
	bool World::RemoveSystem()
	{
		for (auto& system : m_systems)
		{
			auto s = dynamic_cast<S*>(system);
			if (s)
			{
				auto it = std::find(m_systems.begin(), m_systems.end(), s);
				if (it != m_systems.end())
				{
					m_systems.erase(it);
					delete s;
					return true;
				}
			}
		}

		return false;
	}
}
