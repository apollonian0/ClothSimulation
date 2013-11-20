#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "gl_camera.h"
#include "util.h"
#include "win32_util.h"

using namespace std;
using namespace camera;
using namespace math;
using namespace util;
using namespace win32_util;

namespace
{
	POINT Cursor_start_pos;

	HCURSOR Custom_cursor = 
		LoadCursor( NULL, IDC_CROSS );

	HCURSOR Default_cursor =
		GetCursor();

	HWND Win_handle	= NULL;

	HWND getProcWinHandle()
	{
		vector< HWND > hwnds;

		getWinHandles( GetCurrentProcessId(), hwnds );

		if( hwnds.size() )
			return hwnds[ 0 ];
		
		return NULL;
	}
}

void 
GLCamera::updateSimulation(
	float dt )
{
	if( update_rot_matrix_ )
		updateRotMatrix();

	vel_ = vel_ + ( rot_matrix_.transpose() * 
		force_total_ ) * dt;

	vel_ = vel_ * ( 1.0f /  
		( vel_damp_coeff_ + dt ) );
	
	pos_ = pos_ + vel_ * dt;

	force_total_ = force_total_ * 0.0f;
}

void
GLCamera::updateInput()
{
	static bool left_btn_toggle = false;
	static int show_cursor_count = 0;
	static bool left_btn_last_down = false;

	if( !Win_handle )
	{
		Win_handle = getProcWinHandle();

		assert( Win_handle );

		if( !Win_handle )
		{
			cerr << "GLCamera::updateInput: ERROR: failed to retrieve " \
				 << "HWND for current process." << endl;
			return;
		}
	}

	RECT client_rect;
	GetWindowRect( Win_handle, &client_rect );
	
	POINT cursor_pos;
	GetCursorPos( &cursor_pos );

	if( !( cursor_pos.x >= client_rect.left &&
		   cursor_pos.x <= client_rect.right &&
		   cursor_pos.y <= client_rect.bottom &&
		   cursor_pos.y >= client_rect.top + 50 ) )
	{
		if( show_cursor_count < 0 )
			show_cursor_count = 
				ShowCursor( true );
		
		left_btn_toggle = false;

		return;
	}

	SHORT left_btn_down = 
		GetKeyState( VK_LBUTTON );
	
	if( left_btn_down & 0x8000 )
	{
		if( left_btn_last_down )
			return;

		left_btn_last_down = true;
		
		if( !left_btn_toggle )
		{
			left_btn_toggle = true;

			Cursor_start_pos = cursor_pos;
		
			show_cursor_count = 
				ShowCursor( false );
		}
		else
		{
			left_btn_toggle = false;

			ShowCursor( true );
		}
	}
	else
		left_btn_last_down = false;
	
	if( !left_btn_toggle )
		return;

	const float tmp_yaw = yaw_;
	const float tmp_pitch = pitch_;

	yaw_ += ( Cursor_start_pos.x - cursor_pos.x ) * 
		( mouse_damp_coeff_ * ( float )M_PI / 180 );
	
	//	apply pitch to camera
	pitch_ += ( Cursor_start_pos.y - cursor_pos.y ) * 
		( mouse_damp_coeff_ * ( float )M_PI / 180 );

	pitch_ = clamp( pitch_, 
		( float )-M_PI / 2, ( float )M_PI / 2 );

	if( tmp_yaw != yaw_ ||
		tmp_pitch != pitch_ )
		update_rot_matrix_ = true;

	SHORT right_btn_down = 
		GetAsyncKeyState( VK_RBUTTON );

	//	update forward/backward translation
	if( GetAsyncKeyState( VkKeyScan( 'w' ) ) & 0x8000 )
	{
		if( right_btn_down )
			force_total_[ 1 ] += speed_;
		else
			force_total_[ 2 ] -= speed_;
	}
	if( GetAsyncKeyState( VkKeyScan( 's' ) ) & 0x8000 )
	{
		if( right_btn_down )
			force_total_[ 1 ] -= speed_;
		else
			force_total_[ 2 ] += speed_;
	}
	//	update left/right translation
	if( GetAsyncKeyState( VkKeyScan( 'a' ) ) & 0x8000 )
		force_total_[ 0 ] -= speed_;
	if( GetAsyncKeyState( VkKeyScan( 'd' ) ) & 0x8000 )
		force_total_[ 0 ] += speed_;

	SetCursorPos( Cursor_start_pos.x, 
		Cursor_start_pos.y );

	if( show_cursor_count >= 0 )
		show_cursor_count = 
			ShowCursor( false );
}

void 
GLCamera::rotateX( 
	float angle )
{
	update_rot_matrix_ = true;

	pitch_ += angle;
}

void 
GLCamera::rotateY(
	float angle )
{
	update_rot_matrix_ = true;

	yaw_ += angle;
}

void 
GLCamera::loadModelViewMatrix()
{
	updateMVMatrix();

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	GLfloat buf[ 16 ];
	mv_matrix_.ptr16( buf );

	glLoadMatrixf( buf );
}

void 
GLCamera::getModelViewMatrix(
	float *buf )
{
	updateMVMatrix();

	mv_matrix_.ptr16( buf );
}

void
GLCamera::setPerspective(
	float fov_y,
	float aspect,
	float near_z,
	float far_z )
{
	const float f = 1 / tan( fov_y * 0.5f * ( float )M_PI / 180.0f );

	const float delta_z = 
		near_z - far_z;

	proj_matrix_ = 
		Mat4f( 
			Vec4f( f / aspect, 0.0f, 0.0f, 0.0f ),
			Vec4f( 0.0f, f, 0.0f, 0.0f ),
			Vec4f( 0.0f, 0.0f, ( near_z + far_z ) / delta_z, -1.0f ),
			Vec4f( 0.0f, 0.0f, 2 * near_z * far_z / delta_z, 0.0f ) );
}

void
GLCamera::getPerspective(
	float *buf ) const
{
	proj_matrix_.ptr16( buf );
}

math::Vector3< float >
GLCamera::getLookAt()
{
	//updateMVMatrix();
	const Vec4f v4_t = 
		mv_matrix_ * Vec4f( 0.0f, 0.0f, -1.0f, 0.0f );

	return Vec3f( v4_t.x(), v4_t.y(), v4_t.z() );
}

void
GLCamera::updateRotMatrix()
{
	rot_matrix_ =  
		Matrix4< float >::GetIdentity() *
		getYRotationMatrix4< float >( yaw_ ) *
		getXRotationMatrix4< float >( -pitch_ );

	update_rot_matrix_ = false;
}

void
GLCamera::updateMVMatrix()
{
	if( update_rot_matrix_ )
		updateRotMatrix();

	mv_matrix_ =
		Matrix4< float >(
			Vector4< float >( 1.0f, 0.0f, 0.0f, 0.0f ),
			Vector4< float >( 0.0f, 1.0f, 0.0f, 0.0f ),
			Vector4< float >( 0.0f, 0.0f, 1.0f, 0.0f ),
			Vector4< float >( pos_* -1.0f, 1.0f ) ) * rot_matrix_;
}