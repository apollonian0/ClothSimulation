#include "collidable.h"

using namespace std;
using namespace collision;

ostream&
operator<<(
	ostream& os,
	const Collidable& c )
{
	os << "collision_group = " << c.collision_group << ";\t";
	os << "position = " << c.position << ";\t";
	os << "velocity = " << c.velocity << ";\t";
	os << "radius = " << c.radius << ";\t";
	os << "mass = " << c.mass << ";\t";
	return os;
}