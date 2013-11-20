#pragma once

#include <ostream>
#include <vector>
#include <math.h>
#include <assert.h>

namespace math
{

	template < class T > class Vector3
	{

	private:

		//	variables
		T x_;
		T y_;
		T z_;

	public:

		//	ctors
		Vector3();
		Vector3( const T& x, const T& y, const T& z );

		//	dtor
		virtual ~Vector3() {}

		//	functions
		T x() const;
		T y() const;
		T z() const;

		T* ptr3( T *t_arr ) const;
		T* ptr4( T *t_arr ) const;

		virtual T dot( const Vector3< T >& rvalue ) const;
		virtual Vector3< T > operator-( const Vector3< T >& rvalue ) const;
		virtual Vector3< T > operator+( const Vector3< T >& rvalue ) const;
		virtual Vector3< T > operator*( const T scalar ) const;
		virtual Vector3< T >& operator+=( const Vector3< T >& rvalue ); 
		virtual Vector3< T >& operator-=( const Vector3< T >& rvalue ); 
		virtual void operator=( const Vector3< T >& rvalue );
		virtual bool operator==( const Vector3< T >& rvalue ) const;
		virtual Vector3< T > norm() const;
		virtual T len() const;
		virtual Vector3< T > cross( const Vector3< T >& rvalue );
		
		T operator[]( int i ) const;
		T& operator[]( int i );

	};	//	end Vector3

	template < class T >
	inline T math::Vector3< T >::operator[]( int i ) const
	{
		assert( i >= 0 && i < 3 );

		switch( i )
		{
		case 0:
			return x_;
		case 1:
			return y_;
		case 2:
			return z_;
		}

		return 0;
	}

	template < class T >
	inline T& math::Vector3<T>::operator[]( int i )
	{
		assert( i >= 0 && i < 3 );

		switch( i )
		{
		case 0:
			return x_;
		case 1:
			return y_;
		}
		return z_;
	}

	template < class T >
	inline math::Vector3<T>::Vector3()
		:	x_( 0 )
		,	y_( 0 )
		,	z_( 0 )
	{}

	template < class T >
	inline T* math::Vector3<T>::ptr3( T *t_arr ) const
	{
		t_arr[ 0 ] = x_;
		t_arr[ 1 ] = y_;
		t_arr[ 2 ] = z_;
		return t_arr;
	}

	template < class T >
	inline T* math::Vector3<T>::ptr4( T *t_arr ) const
	{
		t_arr[ 0 ] = x_;
		t_arr[ 1 ] = y_;
		t_arr[ 2 ] = z_;
		t_arr[ 3 ] = 0;
		return t_arr;
	}

	template < class T >
	inline math::Vector3<T>::Vector3( 
		const T& x = 0, 
		const T& y = 0, 
		const T& z  = 0 )
		:	x_( x )
		,	y_( y )
		,	z_( z )
	{}

	template < class T >
	inline T math::Vector3<T>::x() const
	{
		return x_;
	}

	template < class T >
	inline T math::Vector3<T>::y() const
	{
		return y_;
	}

	template < class T >
	inline T math::Vector3<T>::z() const
	{
		return z_;
	}

	template < class T >
	inline T math::Vector3<T>::len() const
	{
		return ( T )sqrt( (double)x_*x_ + (double)y_*y_ + (double)z_*z_ );
	}

	template < class T >
	inline Vector3< T > math::Vector3<T>::norm() const
	{
		T len = this->len();
		if( !len )
			return Vector3< T >( 0, 0, 0 );
		return Vector3< T >( x_ / len, y_ / len, z_ / len );
	}

	template < class T >
	inline T math::Vector3<T>::dot( const Vector3< T >& rvalue ) const
	{
		return x_ * rvalue.x_ + y_ * rvalue.y_ + z_ * rvalue.z_;
	}

	template < class T >
	inline Vector3< T > math::Vector3<T>::cross( const Vector3< T >& rvalue )
	{
		T comp_x = y() * rvalue.z() - z() * rvalue.y();
		T comp_y = z() * rvalue.x() - x() * rvalue.z();
		T comp_z = x() * rvalue.y() - y() * rvalue.x();

		return Vector3< T >( comp_x, comp_y, comp_z );
	}

	template < class T >
	inline bool math::Vector3<T>::operator==( const Vector3< T >& rvalue ) const
	{
		return 
			x() == rvalue.x() &&
			y() == rvalue.y() &&
			z() == rvalue.z();
			   
	}

	template < class T >
	inline void math::Vector3<T>::operator=( const Vector3< T >& rvalue )
	{
		x_ = rvalue.x_;
		y_ = rvalue.y_;
		z_ = rvalue.z_;
	}

	template < class T >
	inline Vector3< T > math::Vector3<T>::operator*( const T s) const
	{
		return Vector3< T >( x_*s, y_*s, z_*s );
	}

	template < class T >
	inline Vector3< T >& math::Vector3<T>::operator+=( const Vector3< T >& rvalue )
	{
		x_ += rvalue.x_;
		y_ += rvalue.y_;
		z_ += rvalue.z_;

		return *this;
	}

	template < class T >
	inline Vector3< T >& math::Vector3<T>::operator-=( const Vector3< T >& rvalue )
	{
		return *this += rvalue * -1;
	}

	template < class T >
	inline Vector3< T > math::Vector3<T>::operator+( const Vector3< T >& rvalue ) const
	{
		return Vector3< T >( x_ + rvalue.x_, y_ + rvalue.y_, z_ + rvalue.z_ );
	}

	template < class T >
	inline Vector3< T > math::Vector3<T>::operator-( const Vector3< T >& rvalue ) const
	{
		return Vector3< T >( x_ - rvalue.x_, y_ - rvalue.y_, z_ - rvalue.z_ );
	}

	template < class T >
	inline Vector3< T > getIVector3()
	{
		return Vector3< T >( 1, 0, 0 );
	}

	template < class T >
	inline Vector3< T > getJVector3()
	{
		return Vector3< T >( 0, 1, 0 );
	}

	template < class T >
	inline Vector3< T > getKVector3()
	{
		return Vector3< T >( 0, 0, 1 );
	}

	template< class T >
	inline std::ostream& operator<<( std::ostream& os, const Vector3< T >& vector3 )
	{
		return os << "( " << vector3.x() << ", " << vector3.y() << ", " << vector3.z() << " )";		
	}

	template < class F, class T > 
	inline Vector3< T > vector3ToType( const Vector3< F >& vecf )
	{
		return Vector3< T >( ( T )vecf.x(), ( T )vecf.y(), ( T )vecf.z() );
	}

	template < class T > 
	inline Vector3< T > vector3Floor( const Vector3< T >& vec )
	{
		return Vector3< T >( floor( vec.x() ), floor( vec.y() ), floor( vec.z() ) ) ;
	}

	typedef Vector3< int > Vec3i;
	typedef Vector3< float > Vec3f;
	typedef Vector3< double > Vec3d;

}	//	end math