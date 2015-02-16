#pragma once
#include "ecs.h"

namespace ecs
{
	struct Entity
	{
		ID(Entity);
	};

	struct Component
	{
		ID(Component);

		Component()
		{
			this->OnCreate();
		}

		virtual ~Component()
		{
			this->OnDestroy();
		}

		virtual void OnCreate() {};
		virtual void OnAttach() {};
		virtual void OnRemove() {};
		virtual void OnDestroy() {};
	};
}
