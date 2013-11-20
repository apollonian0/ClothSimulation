#include <algorithm>
#include "integrator.h"
#include "particle.h"

using namespace std;
using namespace particle;

namespace integrator
{
	//	begin SymEuler
	//
	void
	SymEuler::operator()(
		Particle& particle,
		float dt ) const
	{
		particle.last_position = 
			particle.position;

		particle.velocity += particle.force_total * dt;
			
		particle.position += particle.velocity * dt;

		particle.force_total = Vector();
	}
}