#include "collision_info.h"

#include "particle.h"

using namespace std;
using namespace collision;

void
CollisionInfo::initDefault()
{
	time_of_collision = 
	closing_velocity = 
	separating_velocity =
	penetration_distance = 0.0f;

	entity1 = 
	entity2 = NULL;
}

ostream&
operator<<( 
	ostream& os, 
	const CollisionInfo& ci )
{
	os << "time_of_collision = " << ci.time_of_collision << endl;
	os << "closing_velocity = " << ci.closing_velocity << endl;
	os << "separating_velocity = " << ci.separating_velocity << endl;
	os << "contact_normal = " << ci.contact_normal << endl;
	os << "penetration_distance = " << ci.penetration_distance << endl;
	os << "particle1 = " << *ci.entity1 << endl;
	os << "particle2 = " << *ci.entity2 << endl;
	return os;
}