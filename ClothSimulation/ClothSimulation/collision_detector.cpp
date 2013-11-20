#include <algorithm>
#include <fstream>
#include <assert.h>

#include "GL/freeglut.h"

#include "collision_detector.h"
#include "particle.h"

using namespace std;
using namespace collision;
using namespace particle;
using namespace static_geometry;

float
intersectRaySphere(
	const Vector& ray_start,
	const Vector& ray_direction,
	const Vector& sphere_center,
	float sphere_radius )
{
	const Vector pos_delta = 
		ray_start -
		sphere_center;

	const float b = pos_delta.dot( ray_direction );

	const float c = pos_delta.dot( pos_delta ) - 
		sphere_radius * sphere_radius;

	if( c > 0.0f &&
		b > 0.0f )
	{
		return FLT_MAX;
	}

	float disc= 
		b * b - c;

	if( disc < 0.0f  )
	{
		return FLT_MAX;
	}

	const float t = 
		-b - sqrt( disc );

	return t;
}

void
CollisionDetector::appendCollisionInfo(
	Collidable *c1_ptr,
	Collidable *c2_ptr,
	float penetration_dist,
	float t )
{
	const Vector pos_delta =
		c1_ptr->position - c2_ptr->position;

	const Vector pos_delta_norm =
		( c1_ptr->position - c2_ptr->position ).norm();

	float closing_vel =
		( c1_ptr->velocity - c2_ptr->velocity ).dot(
		pos_delta_norm );

	if( c1_ptr->is_static &&
		!c2_ptr->is_static )
	{
		closing_vel = c2_ptr->velocity.dot(
			pos_delta_norm );
	}
	else if( !c1_ptr->is_static &&
			 c2_ptr->is_static )
	{
		closing_vel = c1_ptr->velocity.dot(
			pos_delta_norm );
	}

	CollisionInfo collision;

	collision.entity1 = c1_ptr;
	collision.entity2 = c2_ptr;
	collision.contact_normal = pos_delta_norm;
	collision.penetration_distance = penetration_dist;
	collision.closing_velocity = closing_vel;
	collision.separating_velocity = closing_vel * -1.0f;

	collisions_.push_back( collision );
}

void
CollisionDetector::resolveOutOfWorldCollisions()
{
	for(
		int p1_index = 0;
		p1_index < particle_ptrs_.size();
		++p1_index )
	{
		auto& p1_ptr = particle_ptrs_[ p1_index ];
		assert( p1_ptr );

		const Vector pos =
			p1_ptr->position;

		const float half_world_len = 
			world_len_ / 2;

		const float vel_att = 0.4f;

		if( pos.v[ 0 ] + p1_ptr->radius >= half_world_len )
		{
			const float penetration_dist =
				pos.v[ 0 ] + p1_ptr->radius - half_world_len;
			
			p1_ptr->position.v[ 0 ] -= penetration_dist;
			
			if( p1_ptr->collision_group != 1 )
				p1_ptr->velocity.v[ 0 ] *= -1.0f;
			else
				p1_ptr->velocity.v[ 0 ] = vel_att;
		}

		if( pos.v[ 0 ] - p1_ptr->radius <= -half_world_len )
		{
			const float penetration_dist =
				pos.v[ 0 ] - p1_ptr->radius + half_world_len;
			
			p1_ptr->position.v[ 0 ] -= penetration_dist;
			
			if( p1_ptr->collision_group != 1 )
				p1_ptr->velocity.v[ 0 ] *= -1.0f;
			else
				p1_ptr->velocity.v[ 0 ] *= vel_att;
		}
		
		if( pos.v[ 1 ] + p1_ptr->radius >= half_world_len )
		{
			const float penetration_dist =
				pos.v[ 1 ] + p1_ptr->radius - half_world_len;
			
			p1_ptr->position.v[ 1 ] -= penetration_dist;
			
			if( p1_ptr->collision_group != 1 )
				p1_ptr->velocity.v[ 1 ] *= -1.0f;
			else
				p1_ptr->velocity.v[ 1 ] *= vel_att;
		}

		if( pos.v[ 1 ] - p1_ptr->radius <= -half_world_len )
		{
			const float penetration_dist =
				pos.v[ 1 ] - p1_ptr->radius + half_world_len;
			
			p1_ptr->position.v[ 1 ] -= penetration_dist;
			
			if( p1_ptr->collision_group != 1 )
				p1_ptr->velocity.v[ 1 ] *= -1.0f;
			else
			{
				p1_ptr->velocity = p1_ptr->velocity * vel_att;
				//p1_ptr->mass = 100.0f;
			}
		}
		
		if( pos.v[ 2 ] + p1_ptr->radius >= half_world_len )
		{
			const float penetration_dist =
				pos.v[ 2 ] + p1_ptr->radius - half_world_len;
			
			p1_ptr->position.v[ 2 ] -= penetration_dist;
			
			if( p1_ptr->collision_group != 1 )
				p1_ptr->velocity.v[ 2 ] *= -1.0f;
			else
				p1_ptr->velocity.v[ 2 ] = vel_att;
		}

		if( pos.v[ 2 ] - p1_ptr->radius <= -half_world_len )
		{
			const float penetration_dist =
				pos.v[ 2 ] - p1_ptr->radius + half_world_len;
			
			p1_ptr->position.v[ 2 ] -= penetration_dist;
			
			if( p1_ptr->collision_group != 1 )
				p1_ptr->velocity.v[ 2 ] *= -1.0f;
			else
				p1_ptr->velocity.v[ 2 ] = vel_att;
		}
	}
}

std::vector< CollisionInfo >&
CollisionDetector::detectSweptCollisions(
	float dt )
{
	collisions_.clear();

	detectParticleParticleCollisions( dt );
	resolveOutOfWorldCollisions();
	
	return collisions_;
}

void
CollisionDetector::detectParticleParticleCollisions(
	float dt )
{
	for(
		int p1_index = 0;
		p1_index < particle_ptrs_.size();
		++p1_index )
	{
		auto& p1_ptr = particle_ptrs_[ p1_index ];
		assert( p1_ptr );

		//	clear particle draw color in case
		//	debug color was applied in previous call
		//
		p1_ptr->draw_color = 
			Vector( 0.8f, 0.8f, 0.8f );
		
		for(
			int p2_index = p1_index + 1;
			p2_index < particle_ptrs_.size();
			++p2_index )
		{
			auto& p2_ptr = particle_ptrs_[ p2_index ];
			assert( p2_ptr );

			
			if( !p1_ptr->collidesWith( *p2_ptr ) && 
				!p2_ptr->collidesWith( *p1_ptr ))
				continue;
			

			const float sum_radii =
				p1_ptr->radius + p2_ptr->radius;

			const Vector rel_vel_dir =
				( p2_ptr->velocity - p1_ptr->velocity ).norm();

			const float t = 
				intersectRaySphere( 
					p2_ptr->position,
					rel_vel_dir,
					p1_ptr->position,
					sum_radii );
			
			if( t < dt )
			{
				//cout << "collision!" << endl;
				const float penetration_dist =
					sum_radii - 
					( p2_ptr->position - 
					p1_ptr->position ).len();

				appendCollisionInfo(
					p1_ptr,
					p2_ptr,
					penetration_dist,
					t );

				p1_ptr->draw_color = 
					Vector( 1.0f, 0.0f, 0.0f );

				p2_ptr->draw_color = 
					Vector( 1.0f, 0.0f, 0.0f );
			}
		}
	}
}

bool
willCollideFirst(
	const collision::CollisionInfo& c1,
	const collision::CollisionInfo& c2 )
{
	return c1.time_of_collision < c2.time_of_collision;
}

void
CollisionDetector::printCollisionsDebug(
	ofstream &ofs )
{
	assert( ofs.good() );

	if( !collisions_.empty() )
	{
		sort( collisions_.begin(), collisions_.end(), 
			willCollideFirst );

		for_each( collisions_.begin(), collisions_.end(),
			[&] ( CollisionInfo& collision )
		{
			ofs << collision << endl;
		} );
		ofs << endl;
	}
}

void
CollisionDetector::drawWorld() const
{
	glLineWidth( 15.0f );
	
	glColor4f( 0.5f, 0.5f, 0.1f, 0.8f );

	glutWireCube( world_len_ );
	
	glLineWidth( 1.0f );
}