#include <algorithm>
#include <assert.h>

#include "GL/freeglut.h"

#include "cloth.h"
#include "collision_detector.h"

using namespace std;
using namespace material;
using namespace particle;
using namespace collision;

void 
Cloth::updateSimulation( 
	float dt )
{
	for_each( particles_.begin(), particles_.end(),
		[&] ( Particle& particle )
	{
		if( !particle.is_attached )
		{
			particle.force_total += global_force_;
		
			particle.updateSimulation( dt );
		}
	} );

	updateSpringForces(
		stretch_constraints_,
		stretch_k_,
		0.0f );
	
	satisfyConstraints(
		stretch_constraints_,
		stretch_num_iterations_ );

	updateSpringForces(
		shear_constraints_,
		shear_k_,
		0.0f );
	
	satisfyConstraints(
		shear_constraints_,
		shear_num_iterations_ );
}

void
Cloth::updateSpringForces(
	vector< Constraint >& constraints,
	float k,
	float c )
{
	c = 1.0f;
	for_each( constraints.begin(), constraints.end(),
		[&] ( Constraint& constraint )
	{
		Vector p1 = constraint.p1->position;
		Vector p2 = constraint.p2->position;

		const auto dir = p1 - p2;
		const float dist = 
			dir.len() - constraint.rest_len;

		const float spring_force = k * dist;
		const float damping_force = ( constraint.p1->velocity - 
			constraint.p2->velocity ).len() * c;
		
		if( !constraint.p1->is_attached )
			constraint.p1->force_total += dir.norm() *
				-( spring_force + damping_force );

		if( !constraint.p2->is_attached )
			constraint.p2->force_total -= dir.norm() *
				-( spring_force + damping_force );
	} );
}

void
Cloth::satisfyConstraints(
	vector< Constraint >& constraints,
	int num_iterations )
{
	for( int i = 0; i < num_iterations; ++i )
	{
		for_each( 
			constraints.begin(), 
			constraints.end(),
			[&] ( const Constraint& constraint )
		{
			if( !constraint.p1->is_attached || 
				!constraint.p2->is_attached )
			{
				const auto delta = 
					constraint.p2->position - 
					constraint.p1->position;

				const float delta_len = delta.len();

				const auto delta_norm = delta.norm();

				const float rest_delta = 
					( delta_len - constraint.rest_len )
						/ delta_len;

				if( constraint.p1->is_attached )
					constraint.p2->position -= delta * rest_delta;
				else if( constraint.p2->is_attached )
					constraint.p1->position += delta * rest_delta;
				else
				{
					constraint.p1->position += delta * rest_delta * 0.5f;
					constraint.p2->position -= delta * rest_delta * 0.5f;
				}
			}
		} );
	}
}

void
Cloth::unattach()
{
	if( !is_attached_ )
		return;

	is_attached_ = false;
	
	stretch_k_ *= 0.5f;
	shear_k_ *= 1.3f;

	stretch_num_iterations_ = 1;
	shear_num_iterations_ = 5;

	for_each( particles_.begin(), particles_.end(),
		[&] ( Particle& particle )
	{
		particle.is_attached = false;
	} );
}

void
Cloth::clearConstraints()
{
	stretch_constraints_.clear();
	stretch_constraints_.reserve( 300 );

	shear_constraints_.clear();
	shear_constraints_.reserve( 300 );
}

void
Cloth::initStretchConstraints()
{
	assert( particles_.size() );
	
	stretch_constraints_.clear();
	stretch_constraints_.reserve( 300 );

	const float x_rest = width_ / num_cols_;
	const float y_rest = height_ / num_rows_;
	const float diag_rest = sqrt( 
		x_rest * x_rest + y_rest * y_rest );

	for( int row = 0; row < num_rows_; ++row )
	{
		for( int col = 0; col < num_cols_; ++col )
		{
			int cur_index = row * num_cols_ + col;
		
			if( col < num_cols_ - 1 )
			{
				//	+x edge
				stretch_constraints_.push_back( Constraint(
					&particles_[ cur_index ],
					&particles_[ cur_index + 1 ],
					x_rest ) );
			}
			
			if( col > 0 )
			{
				//	-x edge
				stretch_constraints_.push_back( Constraint(
					&particles_[ cur_index ],
					&particles_[ cur_index - 1 ],
					x_rest ) );
			}
			
			if( row < num_rows_ - 1 )
			{
				//	+y edge
				stretch_constraints_.push_back( Constraint(
					&particles_[ cur_index ],
					&particles_[ cur_index + num_cols_ ],
					y_rest ) );
			}

			if( row > 0 )
			{
				//	-y edge
				stretch_constraints_.push_back( Constraint(
					&particles_[ cur_index ],
					&particles_[ cur_index - num_cols_ ],
					y_rest ) );
			}
		}
	}
}

void
Cloth::initShearConstraints()
{
	assert( particles_.size() );
	
	shear_constraints_.clear();
	shear_constraints_.reserve( 300 );

	const float x_rest = width_ / num_cols_;
	const float y_rest = height_ / num_rows_;
	const float diag_rest = sqrt( 
		x_rest * x_rest + y_rest * y_rest );

	for( int row = 0; row < num_rows_; ++row )
	{
		for( int col = 0; col < num_cols_; ++col )
		{
			int cur_index = row * num_cols_ + col;
			
			if( row > 0 && col > 0 )
			{
				//	-x-y edge
				shear_constraints_.push_back( Constraint(
					&particles_[ cur_index - 1],
					&particles_[ cur_index - num_cols_ ],
					diag_rest ) );
			}

			if( row < num_rows_ - 1 && col < num_cols_ - 1 )
			{
				//	+x+y edge
				shear_constraints_.push_back( Constraint(
					&particles_[ cur_index + 1],
					&particles_[ cur_index + num_cols_ ],
					diag_rest  ) );
			}

			if(  row < num_rows_ - 1 && col > 0 )
			{
				//	-x+y edge
				shear_constraints_.push_back( Constraint(
					&particles_[ cur_index - 1],
					&particles_[ cur_index + num_cols_ ],
					diag_rest ) );
			}

			if( row > 0 && col < num_cols_ - 1 )
			{
				//	+x-y edge
				shear_constraints_.push_back( Constraint(
					&particles_[ cur_index + 1],
					&particles_[ cur_index - num_cols_ ],
					diag_rest ) );
			}

			if( col > 1 && col < num_cols_ - 2 )
			{
				//	++x edge
				shear_constraints_.push_back( Constraint(
					&particles_[ cur_index ],
					&particles_[ cur_index + 2 ],
					2 * x_rest ) );
			}
			
			if( col > 1 && col < num_cols_ - 2 )
			{
				//	--x edge
				shear_constraints_.push_back( Constraint(
					&particles_[ cur_index ],
					&particles_[ cur_index - 2 ],
					2 * x_rest ) );
			}
			
			if( row > 1 && row < num_rows_ - 2 )
			{
				//	++y edge
				shear_constraints_.push_back( Constraint(
					&particles_[ cur_index ],
					&particles_[ cur_index + 2 * num_cols_ ],
					2 * y_rest ) );
			}
			
			if( row > 1 && row < num_rows_ - 2 )
			{
				//	--y edge
				shear_constraints_.push_back( Constraint(
					&particles_[ cur_index ],
					&particles_[ cur_index - 2 * num_cols_ ],
					2 * y_rest ) );
			}
		}
	}
}

void 
Cloth::initGrid( 
	size_t num_rows,
	size_t num_cols,
	float width,
	float height,
	bool is_vertical )
{
	assert( num_rows > 1 );
	assert( num_cols > 1 );

	particles_.clear();
	particles_.reserve( num_rows * num_cols );

	const float x_offset = width / num_cols;
	const float y_offset = height / num_rows;

	const float half_width = width / 2;
	const float half_height = height / 2;

	for( int row = num_rows - 1; row >= 0; --row )
	{
		for( int col = 0; col < num_cols; ++col )
		{
			const Vector pos =
				is_vertical ?
					Vector( col * x_offset - half_width,
							row * y_offset,
							0.0f ) :
					Vector( col * x_offset - half_width,
							0.0f,
							row * y_offset - half_height );

			particles_.push_back(
				Particle( pos ) );

			particles_.back().collision_group = 1;
		}
	}

	for( int i = 0; i < num_cols; ++i )
	{
		particles_[ i ].is_attached = true;
		particles_[ i ].mass = 10000.0f;

		if( !is_vertical )
		{
			const int i2 = 
				( num_rows - 1 ) * num_cols + i;
			particles_[ i2 ].mass = 10000.0f;
			particles_[ i2 ].is_attached = true;
		}
	}
}

void
Cloth::draw() const
{
	//	draw particles
	//
	for_each( particles_.begin(), particles_.end(),
		[&] ( const Particle& particle )
	{
		particle.draw();
	} );

	//	draw edges
	//
	glLineWidth( stretch_k_ / 1000.0f );

	glBegin( GL_LINES );
	{
		
		glColor4f( 0.0f, 1.0f, 0.0f, 0.5f );

		for_each( 
			stretch_constraints_.begin(), 
			stretch_constraints_.end(),
			[&] ( const Constraint& constraint )
		{
			assert( constraint.p1 );
			assert( constraint.p2 );

			const auto& p1_pos = constraint.p1->position;
			const auto& p2_pos = constraint.p2->position;

			glVertex3f(
				p1_pos.v[ 0 ],
				p1_pos.v[ 1 ],
				p1_pos.v[ 2 ] );

			glVertex3f(
				p2_pos.v[ 0 ],
				p2_pos.v[ 1 ],
				p2_pos.v[ 2 ] );
		} );
	}
	glEnd();

	glLineWidth( shear_k_ / 1000.0f );
	glBegin( GL_LINES );
	{
		glColor4f( 0.0f, 0.0f, 1.0f, 0.5f );

		for_each( 
			shear_constraints_.begin(), 
			shear_constraints_.end(),
			[&] ( const Constraint& constraint )
		{
			assert( constraint.p1 );
			assert( constraint.p2 );

			const auto& p1_pos = constraint.p1->position;
			const auto& p2_pos = constraint.p2->position;

			glVertex3f(
				p1_pos.v[ 0 ],
				p1_pos.v[ 1 ],
				p1_pos.v[ 2 ] );

			glVertex3f(
				p2_pos.v[ 0 ],
				p2_pos.v[ 1 ],
				p2_pos.v[ 2 ] );
		} );
	}
	glEnd();

	glLineWidth( 1.0f );
}

void
Cloth::registerParticles(
	CollisionDetector& collision_detector )
{
	for_each( particles_.begin(), particles_.end(),
		[&] ( Particle& particle )
	{
		collision_detector.registerParticle(
			&particle );
	} );
}

void 
Cloth::reset(
	bool is_vertical )
{
	global_force_ = Vector();
	
	is_attached_ = true;

	initGrid( num_rows_, num_cols_, 
		width_, height_, is_vertical );

	stretch_k_ = 2000.0f;
	shear_k_ = 2000.0f;

	initStretchConstraints();
	initShearConstraints();
}
