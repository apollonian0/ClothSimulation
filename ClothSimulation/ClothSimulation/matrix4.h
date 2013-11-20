#pragma once

#include "vector4.h"
#include <math.h>

namespace math
{
	template < class T > class Vector4;

	template < class T > class Matrix4
	{

	private:

		//	variables
		Vector4< T > v1_, v2_, v3_, v4_;

	public:

		//	ctors
		Matrix4();

		Matrix4( const Vector4< T >& v1, const Vector4< T >& v2, 
			const Vector4< T >& v3 );

		Matrix4( const Vector4< T >& v1, const Vector4< T >& v2, 
			const Vector4< T >& v3, const Vector4< T >& v4  );

		Matrix4( const T& pitch, const T& yaw, const T& roll );

		//	functions
		Vector4< T > v1() const;
		Vector4< T > v2() const;
		Vector4< T > v3() const;
		Vector4< T > v4() const;

		void ptr16( T *t_arr ) const;

		Matrix4< T > operator*( const T& rvalue ) const;
		Matrix4< T > operator*( const Matrix4< T >& rvalue ) const;
		Vector3< T > operator*( const Vector3< T >& rvalue ) const;
		Vector4< T > operator*( const Vector4< T >& rvalue ) const;
		Matrix4< T > operator-( const Matrix4< T >& rvalue ) const;
		Matrix4< T > operator+( const Matrix4< T >& rvalue ) const;
			
		Matrix4< T >& translate( const Vector3< T >& v3 );
		Matrix4< T > transpose() const;
		T determinant() const;
		Matrix4< T > flipHandedness() const;

		static Matrix4< T > GetIdentity();

	};	//	end Matrix4


	template < class T >
	Matrix4< T > math::Matrix4<T>::flipHandedness() const
	{
		//	flip direction of z-axis
		return Matrix4< T >( v1_, v2_ * -1, v3_, v4_ );
	}

	template < class T >
	Matrix4< T >& math::Matrix4<T>::translate( const Vector3< T >& tv )
	{
		 v4_ = v4_ + math::Vector4< T >( tv, 0 );

		return *this;
	}

	template < class T >
	Matrix4< T >::Matrix4()
		:	v1_( 1, 0, 0, 0 )
		,	v2_( 0, 1, 0, 0 )
		,	v3_( 0, 0, 1, 0 )
		,	v4_( 0, 0, 0, 1 )
	{}
	
	//TODO fix this!
	template < class T >
	math::Matrix4<T>::Matrix4( const T& pitch, const T& yaw, const T& roll )
	{
		T cos_yaw = cos( yaw );
		T sin_yaw = sin( yaw );

		T cos_pitch = cos( pitch );
		T sin_pitch = sin( pitch );

		T cos_roll = cos( roll );
		T sin_roll = sin( roll );
			
		Matrix4< T > my( 
			Vector4< T >( cos_yaw, 0, -sin_yaw, 0 ),
			Vector4< T >( 0, 1, 0, 0 ),
			Vector4< T >( sin_yaw, 0, cos_yaw, 0 ),
			Vector4< T >( 0, 0, 0, 1 ) );	

		Matrix4< T > mx( 
			Vector4< T >( 1, 0, 0, 0 ),
			Vector4< T >( 0, cos_pitch, -sin_pitch, 0 ),
			Vector4< T >( 0, -sin_pitch, cos_pitch, 0 ),
			Vector4< T >( 0, 0, 0, 1 ) );
	
		
		Matrix4< T > mz(
			Vector4< T >( cos_roll, -sin_roll, 0, 0 ),
			Vector4< T >( sin_roll, cos_roll, 0, 0 ),
			Vector4< T >( 0, 0, 1, 0 ),
			Vector4< T >( 0, 0, 0, 1 ) );

		Matrix4< T > mf = mx * my * mz;
		
		v1_ = mf.v1_;
		v2_ = mf.v2_;
		v3_ = mf.v3_;
		v4_ = mf.v4_;
	}
	
	template < class T >
	Matrix4< T > math::Matrix4<T>::GetIdentity()
	{
		return Matrix4< T >(
			Vector4< T >( 1, 0, 0, 0 ),
			Vector4< T >( 0, 1, 0, 0 ),
			Vector4< T >( 0, 0, 1, 0 ),
			Vector4< T >( 0, 0, 0, 1 ) );
	}

	template < class T >
	void math::Matrix4< T >::ptr16( T *t_arr ) const
	{
		v1_.ptr4( &t_arr[ 0 ] );
		v2_.ptr4( &t_arr[ 4 ] );
		v3_.ptr4( &t_arr[ 8 ] );
		v4_.ptr4( &t_arr[ 12 ] );
	}


	template < class T >
	Matrix4< T > math::Matrix4<T>::operator*( const T& rvalue ) const
	{
		return Matrix4< T >( v1_ * rvalue, v2_ * rvalue, v3_ * rvalue );
	}

	template < class T >
	Vector3< T > Matrix4< T >::operator*( const Vector3< T >& rvalue ) const
	{
		const Vector3< T > v1( v1_.x(), v2_.x(), v3_.x() );
		const Vector3< T > v2( v1_.y(), v2_.y(), v3_.y() );
		const Vector3< T > v3( v1_.z(), v2_.z(), v3_.z() );

		return Vector3< T >( v1.dot( rvalue ), v2.dot( rvalue ), v3.dot( rvalue ) );
	}

	/*
		NOTE: This function assumes matrix and vector are expressed as column-major
	*/
	template < class T >
	Vector4< T > math::Matrix4<T>::operator*( const Vector4< T >& rvalue ) const
	{
		const Vector4< T > v1( v1_.x(), v1_.y(), v1_.z(), v1_.w() );
		const Vector4< T > v2( v2_.x(), v2_.y(), v2_.z(), v2_.w() );
		const Vector4< T > v3( v3_.x(), v3_.y(), v3_.z(), v3_.w() );
		const Vector4< T > v4( v4_.x(), v4_.y(), v4_.z(), v4_.w() );

		return Vector4< T >( v1.dot( rvalue ), v2.dot( rvalue ), 
			v3.dot( rvalue ), v4.dot( rvalue ) );
	}


	template < class T >
	math::Matrix4< T >::Matrix4(
		const Vector4< T >& v1,
		const Vector4< T >& v2, 
		const Vector4< T >& v3  )
		:	v1_( v1 )
		,	v2_( v2 )
		,	v3_( v3 )
		,	v4_( 0, 0, 0, 1 )
	{}

	template < class T >
	math::Matrix4<T>::Matrix4( const Vector4< T >& v1, const Vector4< T >& v2,
		const Vector4< T >& v3, const Vector4< T >& v4 )
		:	v1_( v1 )
		,	v2_( v2 )
		,	v3_( v3 )
		,	v4_( v4 )
	{}

	template < class T >
	Vector4< T > math::Matrix4<T>::v1() const
	{
		return v1_;
	}

	template < class T >
	Vector4< T > math::Matrix4<T>::v2() const
	{
		return v2_;
	}

	template < class T >
	Vector4< T > math::Matrix4<T>::v3() const
	{
		return v3_;
	}
	
	template < class T >
	Vector4< T > math::Matrix4<T>::v4() const
	{
		return v4_;
	}

	template < class T >
	Matrix4< T > math::Matrix4< T >::operator*( const Matrix4< T >& rvalue ) const
	{

		Vector4< T > rv1( rvalue.v1_.x(), rvalue.v2_.x(), rvalue.v3_.x(), rvalue.v4_.x() );
		Vector4< T > rv2( rvalue.v1_.y(), rvalue.v2_.y(), rvalue.v3_.y(), rvalue.v4_.y() );
		Vector4< T > rv3( rvalue.v1_.z(), rvalue.v2_.z(), rvalue.v3_.z(), rvalue.v4_.z() );
		Vector4< T > rv4( rvalue.v1_.w(), rvalue.v2_.w(), rvalue.v3_.w(), rvalue.v4_.w() );


		return Matrix4< T >( 
			Vector4< T >( rv1.dot( v1_ ), rv2.dot( v1_ ), rv3.dot( v1_ ), rv4.dot( v1_ ) ),
			Vector4< T >( rv1.dot( v2_ ), rv2.dot( v2_ ), rv3.dot( v2_ ), rv4.dot( v2_ ) ),
			Vector4< T >( rv1.dot( v3_ ), rv2.dot( v3_ ), rv3.dot( v3_ ), rv4.dot( v3_ ) ),
			Vector4< T >( rv1.dot( v4_ ), rv2.dot( v4_ ), rv3.dot( v4_ ), rv4.dot( v4_ ) ) );
	}

	template < class T >
	Matrix4< T > math::Matrix4< T >::operator-( const Matrix4< T >& rvalue ) const
	{
		return Matrix4< T >( v1_ - rvalue.v1_, v2_ - rvalue.v2_,
			v3_ - rvalue.v3_ );
	}

	template < class T >
	Matrix4< T > math::Matrix4< T >::operator+( const Matrix4< T >& rvalue ) const
	{
		return Matrix4< T >( v1_ + rvalue.v1_, v2_ + rvalue.v2_,
			v3_ + rvalue.v3_ );
	}

	template < class T > Matrix4< T >
	Matrix4< T >::transpose() const
	{
		return Matrix4< T >( 
			Vector4< T >( v1_.x(), v2_.x(), v3_.x(), v4_.x() ),
			Vector4< T >( v1_.y(), v2_.y(), v3_.y(), v4_.y() ), 
			Vector4< T >( v1_.z(), v2_.z(), v3_.z(), v4_.z() ),  
			Vector4< T >( v1_.w(), v2_.w(), v3_.w(), v4_.w() ) );
	}

	template < class T >
	T math::Matrix4< T >::determinant() const
	{
		return v1_.x() * v2_.y() * v3_.z() - v1_.x() * v2_.z() * v3_.y() +
			v1_.y() * v2_.z() * v3_.x() - v1_.y() * v2_.x() * v3_.z() +
			v1_.z() * v2_.x() * v3_.y() - v1_.z() * v2_.y() * v3_.x();
	}

	template < class T > std::ostream&
		operator<<( std::ostream& os, const Matrix4< T >& m )
	{
		return os << m.v1() << "\n" << m.v2() << "\n" << m.v3() << "\n" << m.v4();
	}


	template < class T > Matrix4< T >
	getXRotationMatrix4( T angle )
	{
		return Matrix4< T > (
			Vector4< T >( 1.0, 0.0, 0.0, 0.0 ),
			Vector4< T >( 0.0, cos( angle ), sin( angle ), 0.0 ),
			Vector4< T >( 0.0, -sin( angle ), cos( angle ), 0.0 ),
			Vector4< T >( 0, 0, 0, 1 ) );
	}

	template < class T > Matrix4< T >
	getYRotationMatrix4( T angle )
	{
		return Matrix4< T > (
			Vector4< T >( cos( angle ), 0.0f, sin( angle ), 0.0f ),
			Vector4< T >( 0.0f, 1.0f, 0.0f, 0.0f ),
			Vector4< T >( -sin( angle ), 0.0f, cos( angle ), 0.0f ) );
	}

	template < class T > Matrix4< T >
	getZRotationMatrix4( T angle )
	{
		return Matrix4< T > (
			Vector4< T >( cos( angle ), sin( angle ), 0.0f, 0.0f ),
			Vector4< T >( -sin( angle ), cos( angle ), 0.0f, 0.0f ),
			Vector4< T >( 0.0f, 0.0f, 1.0f, 0.0f ) );
	}

	template < class T > Matrix4< T >
	getTranslationMatrix4( const Vector3< T >& tv3 )
	{
		return Matrix4< T >( 
			Vector4< T >( 1, 0, 0, 0 ),
			Vector4< T >( 0, 1, 0, 0 ),
			Vector4< T >( 0, 0, 1, 0 ),
			Vector4< T >( tv3.x(), tv3.y(), tv3.z(), 1 ) );
	
	}

	template < class T > Matrix4< T >
	getRotationMatrix4( const Vector3< T >& axis, T angle )
	{
		auto u = axis.norm();

		auto v1 = Vector4< T >( 
			cos( angle ) + u.x() * u.x() * ( 1 - cos( angle ) ),
			u.x() * u.y() * ( 1 - cos( angle ) ) - u.z() * sin( angle ) ,
			u.x() * u.z() * (  1 - cos( angle ) ) + u.y() * sin( angle ), 0 );
		
		auto v2 = Vector4< T >(
			u.y() * u.x() * ( 1 - cos( angle ) ) + u.z() * sin( angle ),
			cos( angle ) + u.y() * u.y() * ( 1 - cos( angle ) ),
			u.y() * u.z() * ( 1 - cos( angle ) ) - u.x() * sin( angle ), 0 );

		auto v3 = Vector4< T >(
			u.z() * u.x() * ( 1 - cos( angle ) ) - u.y() * sin( angle ),
			u.z() * u.y() * ( 1 - cos( angle ) ) + u.x() * sin( angle ),
			cos( angle ) + u.z() * u.z() * ( 1 - cos( angle ) ), 0 );

		auto v4 = Vector4< T >( 0, 0, 0, 1 );

		return Matrix4< T >( v1, v2, v3, v4 );
	}

	
	template< class T > Matrix4< T >
	getRotationMatrix4FromPoint( const Vector3< T >& pt )
	{
		auto b1 = Vector4< T >( pt.norm(), 0 );
		auto b2 = Vector4< T >( getJVector3< T >().cross( b1.getVector3() ), 0 );
		auto b3 = Vector4< T >( b1.getVector3().cross( b2.getVector3().norm() ), 0 );

		return Matrix4< T >( b2, b3, b1, Vector4< T >( 0, 0, 0, 1 ) );
	}

	/*
		NOTE @param:arr is assumed to point to 16 consectutive values of type T
		stored in row-major.
	*/
	template < class T > Matrix4< T >
	getMatrix4FromArray( T *arr )
	{
		return Matrix4< T >(
			Vector4< T >( arr[0], arr[1], arr[2], arr[3] ),
			Vector4< T >( arr[4], arr[5], arr[6], arr[7] ),
			Vector4< T >( arr[8], arr[9], arr[10], arr[11] ),
			Vector4< T >( arr[12], arr[13], arr[14], arr[15] ) );
	}

	typedef Matrix4< float > Mat4f;

}	//	end math