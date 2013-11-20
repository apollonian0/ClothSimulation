#pragma once

#include <iostream>
#include <vector>

#include "collision_info.h"

namespace particle
{
	class Particle;
}

namespace static_geometry
{
	class Plane;
}

namespace collision
{
	class CollisionDetector
	{
	public:

		//	functions
		//
		void
		registerParticle(
			particle::Particle* particle_ptr );

		std::vector< CollisionInfo >&
		detectSweptCollisions(
			float dt );

		std::vector< CollisionInfo >&
		detectCurrentCollisions(
			bool set_particle_debug_color = false );

		void
		clearParticles();

		void
		printCollisionsDebug(
			std::ofstream& ofs );

		void
		drawWorld() const;

		//	ctors
		//
		CollisionDetector(
			float world_len = 75.0f );

	private:

		//	functions
		//
		void
		detectParticleParticleCollisions(
			float dt );
		
		void
		resolveOutOfWorldCollisions();

		void
		appendCollisionInfo(
			collision::Collidable* c1,
			collision::Collidable* c2,
			float penetration_dist,
			float t );

		//	variables
		//
		std::vector< particle::Particle* > particle_ptrs_;

		std::vector< CollisionInfo > collisions_;

		float world_len_;
	};

	inline void
	CollisionDetector::clearParticles()
	{
		particle_ptrs_.clear();
	}

	inline void
	CollisionDetector::registerParticle(
		particle::Particle* particle_ptr )
	{
		particle_ptrs_.push_back( particle_ptr );
	}

	inline
	CollisionDetector::CollisionDetector(
		float world_len )
		:	world_len_( world_len )
	{}
}