#pragma once

#include <vector>
#include "vector.h"

namespace collision
{
	struct Collidable
	{
		//	variables
		//
		Vector 
			position,
			last_position,
			velocity,
			last_velocity,
			force_total,
			draw_color;

		float
			radius,
			mass;

		bool 
			is_attached,
			is_static;

		int collision_group;
		
		std::vector< int > collides_with;
	};
}

std::ostream&
operator<<(
	std::ostream&,
	const collision::Collidable& c );