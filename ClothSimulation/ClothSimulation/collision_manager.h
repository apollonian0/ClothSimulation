#pragma once

#include "collision_detector.h"
#include "collision_resolver.h"
#include "particle.h"

namespace collision
{
	class CollisionManager
	{
	public:

		//	functions
		//
		void
		updateSimulation(
			float dt,
			bool draw_debug );

		void
		registerParticle(
			particle::Particle *particle_ptr );

	private:

		//variables
		//
		CollisionDetector detector_;
		CollisionResolver resolver_;

		std::vector< particle::Particle* > particle_ptrs_;
	};

	inline void
	CollisionManager::registerParticle(
		particle::Particle *particle_ptr )
	{
		particle_ptrs_.push_back( particle_ptr );
	}
}