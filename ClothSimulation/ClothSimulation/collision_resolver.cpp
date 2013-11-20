#include <algorithm>
#include <assert.h>

#include "collision_resolver.h"
#include "particle.h"

using namespace std;
using namespace collision;
using namespace particle;

void
CollisionResolver::resolveFutureCollisions(
	float dt,
	std::vector< CollisionInfo >& collisions ) const
{	
	if( collisions.size() )
	{
		auto &collision = collisions.front();
	}
}

void
CollisionResolver::resolveCollision(
	CollisionInfo& collision ) const
{
	//	check if particles already moving apart
	//
	if( collision.closing_velocity < 0 )
	{
		auto p1_ptr = collision.entity1;
		assert( p1_ptr );
		
		auto p2_ptr = collision.entity2;
		assert( p2_ptr );

		const Vector separating_vel_vec =
			collision.contact_normal * 
			collision.closing_velocity * -1.0f;
		
		if( p1_ptr->is_static &&
			!p2_ptr->is_static )
		{
			p2_ptr->position -= 
				collision.contact_normal * 
				collision.penetration_distance;
			p2_ptr->velocity -= separating_vel_vec;
		}
		else if( !p1_ptr->is_static &&
				 p2_ptr->is_static )
		{
			p1_ptr->position -= 
				collision.contact_normal * 
				collision.penetration_distance;
			p1_ptr->velocity -= separating_vel_vec;
		}
		else
		{
			const float mass_total = 
				p1_ptr->mass + p2_ptr->mass;

			if( !p1_ptr->is_static ||
				!p2_ptr->is_static )
			{
				//	move particles out of contact
				//
				p1_ptr->position += 
					collision.contact_normal * 
					collision.penetration_distance  *
					( p2_ptr->mass / mass_total );
		
				p2_ptr->position -= 
					collision.contact_normal * 
					collision.penetration_distance *
					( p1_ptr->mass / mass_total );

				p1_ptr->velocity += separating_vel_vec * 
					( p2_ptr->mass / mass_total );
		
				p2_ptr->velocity -= separating_vel_vec *
					( p1_ptr->mass / mass_total );
			}
		}
	}
}

void
CollisionResolver::resolveCurrentCollisions(
	std::vector< CollisionInfo >& collisions ) const
{
	for_each( collisions.begin(), collisions.end(),
		[&] ( CollisionInfo& collision )
	{
		//	check if particles already moving apart
		//
		//if( collision.closing_velocity < 0 )
		{
			auto p1_ptr = collision.entity1;
			assert( p1_ptr );
		
			auto p2_ptr = collision.entity2;
			assert( p2_ptr );

			const Vector separating_vel_vec =
				collision.contact_normal * 
				collision.separating_velocity;
		
			if( p1_ptr->is_static &&
				!p2_ptr->is_static )
			{
				p2_ptr->position -= 
					collision.contact_normal * 
					collision.penetration_distance;
				p2_ptr->velocity -= separating_vel_vec;
			}
			else if( !p1_ptr->is_static &&
					 p2_ptr->is_static )
			{
				p1_ptr->position -= 
					collision.contact_normal * 
					collision.penetration_distance;

				const Vector perp_vel_comp = 
					p1_ptr->velocity +  
					p1_ptr->velocity.dot( collision.contact_normal );

				p1_ptr->velocity = 
					separating_vel_vec +
					perp_vel_comp;
			}
			else
			{
				const float mass_total = 
					p1_ptr->mass + p2_ptr->mass;

				if( !p1_ptr->is_static ||
					!p2_ptr->is_static )
				{
					//	move particles out of contact
					//
					p1_ptr->position += 
						collision.contact_normal * 
						collision.penetration_distance  *
						( p2_ptr->mass / mass_total );
		
					p2_ptr->position -= 
						collision.contact_normal * 
						collision.penetration_distance *
						( p1_ptr->mass / mass_total );

					p1_ptr->velocity += separating_vel_vec * 
						( p2_ptr->mass / mass_total );
		
					p2_ptr->velocity -= separating_vel_vec *
						( p1_ptr->mass / mass_total );
				}
			}
		}
	} );
}
