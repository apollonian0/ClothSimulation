#pragma once

#include <vector>
#include "particle.h"

namespace material
{
	struct Constraint
	{
		particle::Particle 
			*p1,
			*p2;
	
		struct Color
		{
			float r, g, b, a;

			Color&
			operator+=( const Color& rv )
			{
				r += rv.r;
				b += rv.b;
				g += rv.g;
				a += rv.a;

				return *this;
			}

			Color( 
				float _r, 
				float _g, 
				float _b, 
				float _a )
				:	r( _r )
				,	g( _g )
				,	b( _b )
				,	a( _a )
			{}
		} color;

		float rest_len;

		/*Constraint( 
			particle::Particle *_p1, 
			particle::Particle *_p2,
			float _rest_len = 1.0f );*/

		
		Constraint( 
			particle::Particle *_p1, 
			particle::Particle *_p2,
			float _rest_len )
			:	p1( _p1 )
			,	p2( _p2 )
			,	rest_len( _rest_len )
			,	color( 1.0f, 1.0f, 1.0f, 0.1f ) 
		{}
	};

	
}