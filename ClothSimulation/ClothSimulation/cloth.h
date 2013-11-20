#pragma once

#include <vector>
#include "vector.h"
#include "constraint.h"
#include "particle.h"

namespace collision
{
	class CollisionDetector;
}

namespace material
{
	class Cloth
	{
	public:
		
		//	functions
		//
		void
		registerParticles(
			collision::CollisionDetector& collision_detector );
		
		std::vector< particle::Particle >&
		particles();
	
		void 
		updateSimulation( 
			float dt );

		void
		draw() const;
		
		void 
		addGlobalForce( 
			const Vector& force );

		void 
		setGlobalForce( 
			const Vector& force );

		void
		setShearSpringStiffness(
			float k );

		void
		setStretchSpringStiffness(
			float k );

		float
		getStretchSpringStiffness() const;

		float
		getShearSpringStiffness() const;

		void
		unattach();

		void 
		reset(
			bool is_vertical );
	
		size_t 
		numRows() const;
		
		size_t 
		numCols() const;
		
		float 
		width() const;
		
		float 
		height() const;

		//	ctors
		Cloth( 
			size_t num_rows, 
			size_t num_cols,
			float width,
			float height );
	private:

		//	functions
		//
		void
		updateSpringForces(
			std::vector< Constraint >& constraints,
			float k,
			float c );
	
		void 
		updatePositions( 
			const integrator::Function& f,
			float dt );
	
		void 
		applyGlobalForce();

		void 
		satisfyConstraints(
			std::vector< Constraint >& constraints,
			int num_iterations );

		void 
		initGrid( 
			size_t num_rows, 
			size_t num_cols,
			float width,
			float height,
			bool is_vertical = false );

		void 
		initShearConstraints();

		void 
		initStretchConstraints();

		void
		clearConstraints();

		//	variables
		//
		std::vector< particle::Particle  > particles_;

		std::vector< Constraint > 
			stretch_constraints_,
			shear_constraints_;

		Vector global_force_;

		size_t 
			num_rows_, 
			num_cols_;

		float 
			shear_k_,
			stretch_k_,
			height_, 
			width_;

		bool
			is_attached_;

		int
			stretch_num_iterations_,
			shear_num_iterations_;
	};

	inline void
	Cloth::setShearSpringStiffness(
		float k )
	{
		shear_k_ = k;
	}

	inline float
	Cloth::getShearSpringStiffness() const
	{
		return shear_k_;
	}

	inline void
	Cloth::setStretchSpringStiffness(
		float k )
	{
		stretch_k_ = k;
	}

	inline float
	Cloth::getStretchSpringStiffness() const
	{
		return stretch_k_;
	}

	inline std::vector< particle::Particle >&
	Cloth::particles()
	{
		return particles_;
	}

	inline size_t
	Cloth::numRows() const
	{
		return num_rows_;
	}

	inline size_t
	Cloth::numCols() const
	{
		return num_cols_;
	}

	inline void Cloth::addGlobalForce(
		const Vector& force )
	{
		global_force_ += force;
	}


	inline void Cloth::setGlobalForce(
		const Vector& force )
	{
		global_force_ = force;
	}

	inline Cloth::Cloth( 
		size_t num_rows, 
		size_t num_cols,
		float width,
		float height )
		:	num_rows_( num_rows )
		,	num_cols_( num_cols )
		,	width_( width )
		,	height_( height )
		,	shear_k_( 2000.0f )
		,	stretch_k_( 2000.0f )
		,	global_force_( 0.0f, 0.0f, 0.0f )
		,	is_attached_( true )
		,	stretch_num_iterations_( 1 )
		,	shear_num_iterations_( 1 )
	{ 
		reset( false ); 
	}
}