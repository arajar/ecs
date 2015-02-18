#pragma once
#include <vector>
#include <map>

namespace ecs
{
	struct world;
	typedef unsigned int entity;

	struct component
	{
		virtual ~component() {}
	};

	struct system
	{
		system(world& world) : world_(world) {}
		virtual void operator()() const {} // render
		virtual void operator()(double delta) const {} // update

		world& world_;
	};

	struct world
	{
		std::map<entity, std::vector<component*>> entities_;
		std::vector<system*> systems_;

		entity createEntity()
		{
			unsigned int id = static_cast<unsigned int>(entities_.size()) + 1;
			entities_[id].reserve(0);
			return id;
		}

		void removeEntity(const entity& id)
		{
			auto found = entities_.find(id);
			if (found != entities_.end())
			{
				for (auto& c : entities_[id])
				{
					delete c;
				}

				entities_[id].clear();
				entities_.erase(id);
			}
		}

		void operator()()
		{
			for (auto& s : systems_)
			{
				s;
			}
		}

		void operator()(double deltaTime)
		{
			for (auto& s : systems_)
			{
				(*s)(deltaTime);
			}
		}

		template<typename c = component>
		c* add(const entity& id)
		{
			if (!has<c>(id))
			{
				auto comp = new c;
				entities_[id].push_back(comp);
				return comp;
			}
			return nullptr;
		}

		template<typename c = component>
		bool has(const entity& id)
		{
			for (const auto& comp : entities_[id])
			{
				if (dynamic_cast<c*>(comp)) { return true; }
			}
			return false;
		}

		template<typename c = component>
		c* get(const entity& id)
		{
			for (auto& comp : entities_[id])
			{
				auto co = dynamic_cast<c*>(comp);
				if (co) { return co; }
			}
			return nullptr;
		}

		template<typename c = component>
		bool remove(const entity& id)
		{
			for (auto& comp : entities_[id])
			{
				auto cc = dynamic_cast<c*>(comp);
				if (cc)
				{
					entities_[id].erase();
					delete cc;
					return true;
				}
			}
			return false;
		}

		template<typename s = system>
		s* add()
		{
			auto sys = new s(*this);
			systems_.push_back(sys);
			return sys;
		}

		template<typename s = system>
		bool has() const
		{
			for (const auto& sys : systems_)
			{
				if (dynamic_cast<s*>(sys)) { return true; }
			}
			return false;
		}

		template<typename s = system>
		s* get()
		{
			system* sys = nullptr;
			for (const auto& sys : systems_)
			{
				sys = dynamic_cast<s*>(sys);
				if (sys) { break; }
			}
			return sys;
		}

		template<typename s = system>
		bool remove()
		{
			for (auto& system : systems_)
			{
				auto sys = dynamic_cast<s*>(system);
				if (sys) { systems_.erase(sys); return true; }
			}
			return false;
		}

		template<typename c = component>
		bool search(std::vector<entity>& vec, entity en)
		{
			for (auto& co : entities_[en])
			{
				auto c0 = dynamic_cast<c*>(co);
				if (c0)
				{
					vec.push_back(en);
					return true;
				}
			}
			return false;
		}

		template<typename c = component, typename...Args>
		std::vector<entity> search()
		{
			std::vector<entity> vec;
			for (auto& e : entities_)
			{
				if (get<c>(e.first) && search<Args...>(vec, e.first))
				{
					continue;
				}
			}
			return vec;
		}
	};
}