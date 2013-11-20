#pragma once

#include <iostream>
#include "collidable.h"
#include "vector.h"
#include "integrator.h"

namespace particle
{
	struct Particle 
		:	public collision::Collidable
	{
		//	functions
		//
		void
		updateSimulation(
			float dt );

		virtual void
		draw() const;

		integrator::Function&
		getIntegrator();

		bool
		collidesWith(
			const Particle& p );

		//	ctors
		//
		Particle();

		Particle(
			const Vector& _position );

		Particle(
			const Vector& _position,
			const Vector& _velocity );

		//	dtor
		//
		~Particle();

	private:
		
		//	functions
		//
		void
		initDefault();

		//	variables
		//
		integrator::SymEuler integrator_;
	};

	inline integrator::Function&
	Particle::getIntegrator()
	{
		return integrator_;
	}

	inline
	Particle::Particle()		
	{
		initDefault();
	}

	inline
	Particle::Particle(
		const Vector& _position )		
	{
		position = _position;
		initDefault();
	}

	inline
	Particle::Particle(
		const Vector& _position,
		const Vector& _velocity )				
	{
		position = _position;
		velocity = _velocity;
		initDefault();
	}

	inline
	Particle::~Particle()
	{}
}