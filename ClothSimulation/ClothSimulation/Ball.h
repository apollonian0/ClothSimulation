#pragma once

#include "integrator.h"
#include "particle.h"

namespace particle
{
	class Ball
		:	public particle::Particle
	{
	public:

		//	functions
		//
		void
		draw() const;

		//	ctors
		//
		Ball(
			const Vector& _position,
			const Vector& _velocity,
			float _radius,
			float _mass );
	};

	inline
	Ball::Ball(
		const Vector& _position,
		const Vector& _velocity,
		float _radius,
		float _mass )
		:	Particle( 
				_position,
				_velocity )
	{
		radius = _radius;
		mass = _mass;
	}
}