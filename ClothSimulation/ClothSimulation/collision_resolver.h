#pragma once

#include <vector>
#include "collision_info.h"

namespace collision
{
	class CollisionResolver
	{
	public:

		//	functions
		//
		void
		resolveFutureCollisions(
			float dt,
			std::vector< CollisionInfo >& collisions ) const;

		void
		resolveCurrentCollisions(
			std::vector< CollisionInfo >& collisions ) const;

		void
		resolveCollision(
			CollisionInfo& collisions ) const;
	};
}