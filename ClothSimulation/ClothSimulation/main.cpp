#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "air_jet.h"
#include "cloth.h"
#include "ball.h"
#include "timer.h"
#include "gl_camera.h"
#include "collision_detector.h"
#include "collision_resolver.h"

#pragma comment( lib, "glew32.lib" )
#pragma comment( lib, "freeglut.lib" )

using namespace std;
//#define DEBUG_COLLISIONS

namespace
{
	const string Collision_debug_file_path		= "..\\log\\collision_debug.txt";

	ofstream Debug_ofs( 
		Collision_debug_file_path );

	const size_t Window_w						= 1200;
	const size_t Window_h						= 800;
	const float Aspect							= Window_w / ( float )Window_h;
	const float Fov_y							= 45.0f;
	const float Near_z							= 0.01f;
	const float Far_z							= 5000.0f;

	const float Ball_radius						= 3.0f;
	const float Ball_mass						= 500.0f;
	const float Ball_speed						= 60.0f;

	const unsigned int Num_rows					= 20;
	const unsigned int Num_cols					= 20;
	const float Cloth_width						= 1.5f * Num_cols;
	const float Cloth_height					= 1.5f * Num_rows;

	material::Cloth Cloth( Num_rows, Num_cols,
		Cloth_width, Cloth_height );

	const float FPS								= 500.0f;
	const float FI								= 1 / FPS;
	float Last_frame_elapsed					= 0.0f;
	float Total_elapsed							= 0.0f;
	
	timing::Timer Timer;

	camera::GLCamera Camera;

	collision::CollisionDetector Collision_detector;
	collision::CollisionResolver Collision_resolver;

	std::vector< particle::Ball > Balls;

	const float Room_len						= 200.0f;

	const Vector Gravity( 0.0f, -15.0f, 0.0f );

	int Stretch_k_param_index					= 0;
	int Shear_k_param_index						= 1;
	int Current_param_index						= Stretch_k_param_index;

	std::map< int, float > Cloth_param_deltas;
	std::map< int, float > Cloth_param_defaults;
	std::map< int, float > Cloth_params;

	std::vector< force::AirJet > 
		Jets_1,
		Jets_2;

	const float Jets_1_radius					= 55.0f;

	force::AirJet Jet(
		500.0f,
		Vector( 0.0f, 0.0f, 0.0f ).norm(),
		Vector( 0.0f , -40.0f, 0.0f ),
		0.0f,
		50.0f );

	bool Apply_grav_to_balls					= true;
	bool Apply_jets_to_balls					= true;
	bool Apply_jets								= true;

	float Jet_force								= 400.0f;
	float Jet_height							= -40.0f;
}

void
initClothParams()
{
	Cloth_param_defaults[ Stretch_k_param_index ] = 
		Cloth.getStretchSpringStiffness();
	Cloth_param_defaults[ Shear_k_param_index ] = 
		Cloth.getShearSpringStiffness();

	Cloth_param_deltas[ Stretch_k_param_index ] = 250.0f;
	Cloth_param_deltas[ Shear_k_param_index ] = 250.0f;

	Cloth_params[ Stretch_k_param_index ] = 
		Cloth.getStretchSpringStiffness();

	Cloth_params[ Shear_k_param_index ] = 
		Cloth.getShearSpringStiffness();
}

void
updateClothParams()
{
	Cloth.setShearSpringStiffness(
		Cloth_params[ Shear_k_param_index ] );

	Cloth.setStretchSpringStiffness(
		Cloth_params[ Stretch_k_param_index ] );
}

void
initJets()
{
	const float dist_cutoff = 50.0f;

	force::AirJet jet1(
		Jet_force,
		Vector( 0.0f, 0.0f, 0.0f ).norm(),
		Vector( 1.0f, -30.0f, 0.0f ),
		0.9f,
		dist_cutoff );

	jet1.phase_delta = 0.0f;

	Jets_1.push_back( jet1 );

	force::AirJet jet2(
		Jet_force,
		Vector( 0.0f, 0.0f, 0.0f ).norm(),
		Vector( 0.0f, -30.0f, 1.0f ),
		0.9f,
		dist_cutoff );
	
	jet2.phase_delta = ( float )M_PI / 2;

	Jets_1.push_back( jet2 );

	force::AirJet jet3(
		Jet_force,
		Vector( 0.0f, 0.0f, 0.0f ).norm(),
		Vector( 0.0f, -30.0f, 1.0f ),
		0.9f,
		dist_cutoff );
	
	jet3.phase_delta = ( float )M_PI;

	Jets_1.push_back( jet3 );

	force::AirJet jet4(
		Jet_force,
		Vector( 0.0f, 0.0f, 0.0f ).norm(),
		Vector( 0.0f, -30.0f, 1.0f ),
		0.9f,
		dist_cutoff );
	
	jet4.phase_delta = ( float )M_PI / - 2;;

	Jets_1.push_back( jet4 );
}

void
updateJets()
{
	for_each( Jets_1.begin(), Jets_1.end(),
		[&] ( force::AirJet& jet )
	{
		const Vector pos = 
			jet.getPosition();

		jet.setDirection(
			Vector( 
				-cos( Total_elapsed ),
				 0.0f,
				-sin( Total_elapsed ) ).norm() );

		jet.setPosition(
			Vector( 
				cos( Total_elapsed + jet.phase_delta ) * Jets_1_radius,
				Jet_height,
				sin( Total_elapsed + jet.phase_delta ) * Jets_1_radius ) );

	} );
}

void
spawnBall(
	const Vector& position,
	const Vector& velocity )
{
	Balls.push_back( 
		particle::Ball( 
			position,
			velocity,
			Ball_radius,
			Ball_mass ) );

	Balls.back().collides_with.push_back( 0 );
	Balls.back().collides_with.push_back( 1 );

	Collision_detector.registerParticle( 
		&Balls.back() );
}

void
spawnBallAtCamera()
{
	const math::Vec3f vel =
		Camera.getLookAt().norm() * Ball_speed;

	Vector position( 
		Camera.getPosition().x(),
		Camera.getPosition().y(),
		Camera.getPosition().z() );

	Vector velocity = Vector( 
		vel.x(), vel.y(), vel.z() );

	spawnBall(
		position,
		velocity );
}

void
initCollisionDetector()
{
	Cloth.registerParticles(
		Collision_detector );
}

void initCamera()
{
	Camera.setPosition( 
		0.0f,
		0.0f, 
		130.0f );
}

void
initBalls()
{
	Balls.clear();
}

void 
initGL()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( Fov_y, Aspect, Near_z, Far_z );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );

	glEnable( GL_DEPTH_TEST );

	glShadeModel( GL_SMOOTH );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glPointSize( 3 );
}

void
resetSimulation(
	bool cloth_is_vertical )
{
	Cloth_params[ Stretch_k_param_index ] = 
		Cloth_param_defaults[ Stretch_k_param_index ];
	
	Cloth_params[ Shear_k_param_index ] = 
		Cloth_param_defaults[ Shear_k_param_index ];

	Collision_detector.clearParticles();
	Cloth.reset( cloth_is_vertical );
	Cloth.registerParticles( 
		Collision_detector );
	initBalls();
	initCamera();
}

void
drawBalls()
{
	for_each( Balls.begin(), Balls.end(),
		[&] ( const particle::Ball& ball )
	{
		ball.draw();
	} );
}

void
updateBalls( 
	float dt )
{
	for_each( Balls.begin(), Balls.end(),
		[&] ( particle::Ball& ball )
	{
		if( Apply_grav_to_balls )
			ball.force_total += Gravity * 3.0f;

		ball.updateSimulation( dt );
	} );
}

void
drawJets()
{
	for_each( Jets_1.begin(), Jets_1.end(),
		[&] ( force::AirJet& jet )
	{
		const float j_force =
			jet.getForce();

		const float j_dist_cutoff =
			jet.getDistCutoff();

		const float j_cutoff= 
			jet.getCosCutoff();

		const Vector j_pos = 
			jet.getPosition();

		const Vector j_dir= 
			jet.getDirection();

		glColor4f( 0.0f, 0.0f, 1.0f, 0.2f );

		const float angle_rad = 
			asin( 1.0f - j_cutoff );

		const Vector look_at = 
			( j_dir - j_pos ).norm();

		const float dir_yaw_deg =
			atan2( look_at.norm().v[ 2 ], 
			look_at.norm().v[ 0 ] ) * 180.0f /  ( float )M_PI
			+ 90.0f;

		const float dir_pitch_deg =
			-90.0 + acos( look_at.dot( Vector( 0.0f, 1.0f, 0.0f ) ) )* 
			180.0f /  ( float )M_PI;

		glPushMatrix();
	
		glTranslatef( 
			j_pos.v[ 0 ],
			j_pos.v[ 1 ],
			j_pos.v[ 2 ] );
	
		glRotatef( -dir_yaw_deg, 0.0f, 1.0f, 0.0f );
		glRotatef( -dir_pitch_deg, 1.0f, 0.0f, 0.0f );

		glLineWidth( 4.0f );

		glutWireCone( 
			abs( angle_rad ) * j_dist_cutoff * 2, 
			j_dist_cutoff, 10, 10 );

		glPopMatrix();
	} );
}

void
applyJets()
{
	if( !Apply_jets )
		return;

	auto& particles = 
		Cloth.particles();

	for_each( Jets_1.begin(), Jets_1.end(),
		[&] ( force::AirJet& jet )
	{
		for_each( particles.begin(), particles.end(),
			[&] ( particle::Particle& particle )
		{
			const float j_force =
				jet.getForce();

			const Vector j_pos = 
				jet.getPosition();

			const Vector j_dir= 
				jet.getDirection();

			const Vector p_offset = 
				particle.position - j_pos;

			const Vector p_offset_dir = 
				p_offset.norm();

			const float dist_cutoff =
				jet.getDistCutoff();

			const auto cos = p_offset.norm().dot( j_dir );

			if( abs( p_offset.norm().dot( j_dir ) ) >= 
				jet.getCosCutoff() )
			{
				if( p_offset.dot( p_offset ) <= 
					dist_cutoff * dist_cutoff ) 
				{
					const float dist_att = 
						p_offset.len() / dist_cutoff;

					particle.force_total += p_offset_dir *
						j_force;// * ( 1.0f - dist_att );
				}
			}
		} );
	} );

	if( Apply_jets_to_balls )
	{
		for_each( Jets_1.begin(), Jets_1.end(),
			[&] ( force::AirJet& jet )
		{
			for_each( Balls.begin(), Balls.end(),
				[&] ( particle::Ball& ball )
			{
				const float j_force =
					jet.getForce();

				const Vector j_pos = 
					jet.getPosition();

				const Vector j_dir= 
					jet.getDirection();

				const Vector p_offset = 
					ball.position - j_pos;

				const Vector p_offset_dir = 
					p_offset.norm();

				const float dist_cutoff =
					jet.getDistCutoff();

				const auto cos = p_offset.norm().dot( j_dir );

				if( abs( p_offset.norm().dot( j_dir ) ) >= 
					jet.getCosCutoff() )
				{
					if( p_offset.dot( p_offset ) <= 
						dist_cutoff * dist_cutoff ) 
					{
						const float dist_att = 
							p_offset.len() / dist_cutoff;

						ball.force_total += p_offset_dir *
							j_force;// * ( 1.0f - dist_att );
					}
				}
			} );
		} );
	}
}

void
updateSimulation(
	float dt )
{
	Cloth.setGlobalForce( 
		Gravity );

	updateJets();

	applyJets();

	updateBalls( dt );

	Cloth.updateSimulation( dt );

	auto& collisions =
		Collision_detector.detectSweptCollisions( dt );
	
	Collision_resolver.resolveCurrentCollisions(
		collisions );
}

void
drawFPSGraph()
{
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f );

	glLineWidth( 5.0f );

	const float clamped_fps = 
		min( 1 / Last_frame_elapsed, FPS );

	const float norm_fps = 
		clamped_fps / FPS;

	glBegin( GL_LINES );
	{
		glColor3f( 0.0f, 1.0f, 0.0f );
		glVertex2f( 0.0f, 1.0f );
		glVertex2f( norm_fps, 1.0f );
	}
	glEnd();

	glLineWidth( 1.0f );

	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

void 
updateFrame()
{
	static int frame_count = 0;

	if( Timer.getStopWatchSec() >= FI )
	{
		++frame_count;

		Last_frame_elapsed = 
			Timer.getStopWatchSec();
		
		Timer.startStopwatch();
		
		Total_elapsed += Last_frame_elapsed;

		//	update all particles while applying collision detection 
		//	and resolution
		//
		updateSimulation( Last_frame_elapsed );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		glLoadIdentity();

		Camera.updateInput();
		Camera.updateSimulation( Last_frame_elapsed );
		Camera.loadModelViewMatrix();
		
		Cloth.draw();

		glPushMatrix();

		drawJets();

		drawBalls();

		Collision_detector.drawWorld();

		drawFPSGraph();

		glutSwapBuffers();
	}
	
	glutPostRedisplay();
}

void
keyboardFunc( 
	unsigned char key, 
	int x, 
	int y )
{
	switch( toupper( key ) )
	{
	case 27:
		exit(0);
		break;
	case '1':
		resetSimulation(
			true );
		break;
	case '2':
		resetSimulation(
			false );
		break;
	case '8':
		Apply_grav_to_balls = !Apply_grav_to_balls;
		break;
	case '0':
		Apply_jets = !Apply_jets;
		break;
	case 'B':
		spawnBallAtCamera();
		return;
	case 'K':
		Current_param_index = Stretch_k_param_index;
		break;
	case 'L':
		Current_param_index = Shear_k_param_index;
		break;
	case 'U':
		Cloth.unattach();
		break;
	case '+':
		Cloth_params[ Current_param_index ] += 
			Cloth_param_deltas[ Current_param_index ];
		updateClothParams();
		break;
	case '-':
		Cloth_params[ Current_param_index ] -= 
			Cloth_param_deltas[ Current_param_index ];
		updateClothParams();
	case '[':
		Jet_height++;
		break;
	case ']':
		Jet_height--;
		break;
	};
}

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE );
	glutInitWindowSize( Window_w, Window_h );
	glutInitWindowPosition( 
		1920 - Window_w - 20, 10 );

	glutCreateWindow( "GLHelper" );

	glutDisplayFunc( updateFrame );
	glutKeyboardFunc( keyboardFunc );

	glewInit();

	initJets();
	initClothParams();
	initCollisionDetector();
	initCamera();
	initGL();
	
	Balls.reserve( 200 );

	Timer.startStopwatch();

	glutMainLoop();

	return 0;
}