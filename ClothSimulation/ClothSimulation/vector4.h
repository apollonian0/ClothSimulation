#pragma once

#include <ostream>
#include <vector>

#include "vector3.h"

namespace math
{
	
	template < class T > class Matrix4;

	template < class T > class Vector4
	{

	private:

		//	variables
		T x_, y_, z_, w_;

	public:

		//	ctors
		Vector4( const T& x = 0, const T& y = 0, const T& z = 0, const T& w = 0 );
		Vector4( const Vector3< T > &v3, const T& w );

		//	dtor
		virtual ~Vector4() {}

		//	functions
		T x() const;
		T y() const;
		T z() const;
		T w() const;

		void ptr4( T *t_arr ) const;
		Vector3< T > getVector3() const;

		virtual T dot( const Vector4< T >& rvalue ) const;
		virtual Vector4< T > operator-( const Vector4< T >& rvalue ) const;
		virtual Vector4< T > operator+( const Vector4< T >& rvalue ) const;
		virtual Vector4< T >& operator+=( const Vector4< T >& rvalue );
		virtual Vector4< T > operator*( const T scalar ) const;
		virtual void operator=( const Vector4< T >& rvalue );
		virtual bool operator==( const Vector4< T >& rvalue ) const;
		virtual Vector4< T > norm() const;
		virtual T len() const;
		virtual Vector4< T > cross( const Vector4< T >& rvalue );

		virtual T& operator[]( int i );
		virtual const T& operator[]( int i ) const;

	};	//	end Vector4

	template < class T >
	void math::Vector4<T>::ptr4( T *t_arr ) const
	{
		t_arr[ 0 ] = x_;
		t_arr[ 1 ] = y_;
		t_arr[ 2 ] = z_;
		t_arr[ 3 ] = w_;
	}

	template < class T > Vector3< T > 
	Vector4< T >::getVector3() const
	{
		return Vector3< T >( x_, y_, z_ );
	}

	template < class T >
	math::Vector4<T>::Vector4( 
		const T& x = 0, 
		const T& y = 0, 
		const T& z = 0,
		const T& w = 0 )
		:	x_( x )
		,	y_( y )
		,	z_( z )
		,	w_( w )
	{}

	template < class T >
	math::Vector4< T >::Vector4( const Vector3< T > &v3, const T& w = 0 )
		:	x_( v3.x() )
		,	y_( v3.y() )
		,	z_( v3.z() )
		,	w_(	w )
	{}

	template < class T >
	T math::Vector4<T>::x() const
	{
		return x_;
	}

	template < class T >
	T math::Vector4<T>::y() const
	{
		return y_;
	}

	template < class T >
	T math::Vector4<T>::z() const
	{
		return z_;
	}

	template < class T >
	T math::Vector4<T>::w() const
	{
		return w_;
	}

	template < class T >
	T math::Vector4<T>::len() const
	{
		T xn = x_ / w_;
		T yn = y_ / w_;
		T zn = z_ / w_;

		if( w_ )
		{
			xn /= w_;
			yn /= w_;
			zn /= w_;
		}

		return sqrt( xn * xn + yn * yn + zn * zn );
	}

	template < class T >
	Vector4< T > math::Vector4<T>::norm() const
	{		
		if( w_ )
			return Vector4< T >( Vector3< T >( x_ / w_, y_ / w_, z_ / w_ ).norm(), 1 );
		else
			return Vector4< T >( Vector3< T >( x_, y_, z_ ).norm(), 0 );
	}

	template < class T >
	T math::Vector4<T>::dot( const Vector4< T >& rvalue ) const
	{
		return x_ * rvalue.x_ + y_ * rvalue.y_ + z_ * rvalue.z_ + rvalue.w_ * w_;
	}

	template < class T >
	Vector4< T > math::Vector4<T>::cross( const Vector4< T >& rvalue )
	{
		T comp_x = y() * rvalue.z() - z() * rvalue.y();
		T comp_y = z() * rvalue.x() - x() * rvalue.z();
		T comp_z = x() * rvalue.y() - y() * rvalue.x();

		return Vector4< T >( comp_x, comp_y, comp_z, w_ );
	}

	template < class T >
	bool math::Vector4<T>::operator==( const Vector4< T >& rvalue ) const
	{
		return 
			x() == rvalue.x() &&
			y() == rvalue.y() &&
			z() == rvalue.z() &&
			w() == rvalue.w();
			   
	}

	template < class T >
	void math::Vector4<T>::operator=( const Vector4< T >& rvalue )
	{
		x_ = rvalue.x_;
		y_ = rvalue.y_;
		z_ = rvalue.z_;
		w_ = rvalue.w_;
	}

	template < class T >
	Vector4< T > math::Vector4<T>::operator*( const T s) const
	{
		return Vector4< T >( x_ * s, y_ * s, z_ * s, w_ * s );
	}

	template < class T >
	Vector4< T > math::Vector4<T>::operator+( const Vector4< T >& rvalue ) const
	{
		return Vector4< T >( x_ + rvalue.x_, y_ + rvalue.y_, z_ + rvalue.z_, w_ + rvalue.w_ );
	}

	template < class T >
	Vector4< T >& math::Vector4<T>::operator+=( const Vector4< T >& rvalue )
	{
		*this = *this + rvalue;
		return *this;
	}

	template < class T >
	Vector4< T > math::Vector4<T>::operator-( const Vector4< T >& rvalue ) const
	{
		return Vector4< T >( x_ - rvalue.x_, y_ - rvalue.y_, z_ - rvalue.z_, w_ - rvalue.w_ );
	}

	template < class T >
	T& Vector4< T >::operator[]( int i )
	{
		assert( i >= 0 && i <= 3 );
		return 
			i == 0 ? x_ : 
			i == 1 ? y_ :
			i == 2 ? z_ :
			w_;
	}

	template < class T >
	const T& Vector4< T >::operator[]( int i ) const
	{
		return 
			i == 0 ? x_ : 
			i == 1 ? y_ :
			i == 2 ? z_ :
			w_;
	}

	template< class T >
	std::ostream& operator<<( std::ostream& os, const Vector4< T >& v4 )
	{
		return os << "( " << v4.x() << ", " << v4.y() \
				  << ", " << v4.z() << ", " << v4.w() << " )";		
	}

	typedef Vector4< double > Vec4d;
	typedef Vector4< float > Vec4f;
	typedef Vector4< int > Vec4i;

}	//	end math