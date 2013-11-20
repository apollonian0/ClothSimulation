#pragma once

#include "vector.h"

namespace collision
{
	class Collidable;

	struct CollisionInfo
	{
		//	ctors
		//
		CollisionInfo();

		//	variables
		//
		collision::Collidable
			*entity1, 
			*entity2;
		
		Vector contact_normal;

		float
			time_of_collision,
			closing_velocity,
			separating_velocity,
			penetration_distance;
	private:

		//	functions
		//
		void
		initDefault();
	};
	
	inline
	CollisionInfo::CollisionInfo()
	{
		initDefault();
	}
}

std::ostream&
operator<<(
	std::ostream& os, 
	const collision::CollisionInfo& ci );