#include "ball.h"

#include "GL/freeglut.h"

using namespace std;
using namespace particle;

void
Ball::draw() const
{
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
		0.6f );

	glutSolidSphere(
		radius, 20, 20 );

	glPopMatrix();
}