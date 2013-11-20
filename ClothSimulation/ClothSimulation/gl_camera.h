#pragma once

#include "matrix4.h"

namespace camera
{
	class GLCamera
	{
	public:

		//	functions
		void
		updateInput();

		void
		updateSimulation(
			float dt );

		void 
		rotateX( 
			float angle );

		void 
		rotateY(
			float angle );

		const math::Vector3< float >&
		getPosition() const;

		void
		setPosition(
			float x,
			float y,
			float z );

		void 
		loadModelViewMatrix();
		
		void
		getModelViewMatrix(
			float* buf );

		void
		setPerspective(
			float fov,
			float aspect,
			float near_z,
			float far_z );

		void
		getPerspective(
			float *buf
		) const;

		math::Vector3< float >
		getLookAt();
		
		//	ctors
		GLCamera();

	private:

		//	function
		void
		updateBasis();

		void
		updateRotMatrix();

		void
		updateTransMatrix();

		void
		updateMVMatrix();

		//	variables
		math::Matrix4< float > rot_matrix_;
		math::Matrix4< float > mv_matrix_;
		math::Matrix4< float > proj_matrix_;

		math::Vector3< float >
			pos_,
			vel_, 
			rel_translation_,	//	camera space translation vector
			force_total_;

		float 
			pitch_, 
			yaw_, 
			vel_damp_coeff_, 
			mouse_damp_coeff_,
			speed_;

		bool update_rot_matrix_;
		
		bool left_button_down_;	

		float mouse_click_x_, 
			mouse_click_y;
	};

	inline const math::Vector3< float >&
	GLCamera::getPosition() const
	{
		return pos_;
	}

	inline void
	GLCamera::setPosition(
		float x,
		float y,
		float z )
	{
		pos_ = math::Vector3< float >( x, y, z );
		vel_ = vel_ * 0.0f;
		pitch_ = yaw_ = 0.0f;
		update_rot_matrix_ = true;
	}

	inline GLCamera::GLCamera()
		:	pitch_( 0.0f )
		,	yaw_( 0.0f )
		,	vel_damp_coeff_( 1.05f )
		,	mouse_damp_coeff_( 0.3f )
		,	speed_( 575.0f )
	{}
}