#pragma once

#include <vector>

namespace particle
{
	class Particle;
}

namespace integrator
{
	struct Function
	{
		//	operators
		virtual void
		operator()(
			particle::Particle& particle,
			float dt ) const = 0;
	};

	struct SymEuler :
		public Function
	{
		//	Function operators
		virtual void
		operator()( 
			particle::Particle& particle,
			float dt ) const;
	};
}
