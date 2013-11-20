#include <algorithm>

#include "particle.h"

#include "GL/freeglut.h"

using namespace std;
using namespace particle;

void
Particle::updateSimulation(
	float dt )
{
	integrator_( *this, dt );
}

bool
Particle::collidesWith(
	const Particle& p )
{
	for( int i = 0; i < collides_with.size(); ++i )
	{
		if( collides_with[ i ] == p.collision_group )
			return true;
	}
	return false;
}

void
Particle::draw() const
{
	glLineWidth( 2.0f );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	glTranslatef( 
		position.v[ 0 ],
		position.v[ 1 ],
		position.v[ 2 ] );
	
	glColor4f( 
		draw_color.v[ 0 ], 
		draw_color.v[ 1 ], 
		draw_color.v[ 2 ],
		0.1f );

	glutWireSphere(
		radius, 10, 10 );

	glPopMatrix();
}

void
Particle::initDefault()
{
	radius = 0.5f;
	mass = 20.0f;
	draw_color = Vector( 1.0f, 1.0f, 1.0f );
	is_attached = false;
	is_static = false;
	collision_group = 0;
}