#pragma once

#include <iostream>
#include <math.h>

struct Vector
{
	//	variables
	float v[3];

	//	operators
	Vector operator+( const Vector& rv ) const;
	Vector operator-( const Vector& rv ) const;
	Vector& operator+=( const Vector& rv );
	Vector& operator-=( const Vector& rv );
	Vector operator*( float mult ) const;
	Vector operator/( float div ) const;
	bool operator==( const Vector& rv ) const;

	//	functions
	Vector cross( const Vector& rv ) const;
	float dot( const Vector& rv ) const;
	float len() const;
	Vector norm() const;
	float dist( const Vector& rv ) const;

	void copyTo( float *dest );

	//	ctors
	Vector( 
		float _x = 0.0f,
		float _y = 0.0f, 
		float _z = 0.0f );

	Vector( float *arr_ptr );	//	assumes ptr points to a float array with 3 elements

};

inline Vector 
Vector::operator+( 
	const Vector& rv ) const
{
	return Vector( 
		v[ 0 ] + rv.v[ 0 ], 
		v[ 1 ] + rv.v[ 1 ], 
		v[ 2 ] + rv.v[ 2 ] );
}

inline Vector 
Vector::operator-( 
	const Vector& rv ) const
{
	return Vector( 
		v[ 0 ] - rv.v[ 0 ], 
		v[ 1 ] - rv.v[ 1 ], 
		v[ 2 ] - rv.v[ 2 ] );
}

inline Vector
Vector::operator*( 
	float mult ) const
{
	return Vector(
		v[ 0 ] * mult,
		v[ 1 ] * mult,
		v[ 2 ] * mult );
}

inline Vector
Vector::operator/( float div ) const
{
	return *this * ( 1 / div );
}

inline bool 
Vector::operator==( const Vector& rv ) const
{
	return 
		v[ 0 ] == rv.v[ 0 ] &&
		v[ 2 ] == rv.v[ 1 ] &&
		v[ 2 ] == rv.v[ 2 ];
}

inline Vector 
Vector::cross( 
	const Vector& rv ) const
{
	return Vector(
		v[ 1 ] * rv.v[ 2 ] - v[ 2 ] * rv.v[ 1 ],
		v[ 2 ] * rv.v[ 0 ] - v[ 0 ] * rv.v[ 2 ],
		v[ 0 ] * rv.v[ 1 ] - v[ 1 ] * rv.v[ 0 ] );
}

inline float 
Vector::dot( 
	const Vector& rv ) const
{
	return 
		v[ 0 ] * rv.v[ 0 ] +
		v[ 1 ] * rv.v[ 1 ] + 
		v[ 2 ] * rv.v[ 2 ];
}

inline float 
Vector::len() const
{
	return sqrt( 
			v[ 0 ] * v[ 0 ] + 
			v[ 1 ] * v[ 1 ] + 
			v[ 2 ] * v[ 2 ] );	
}

inline Vector
Vector::norm() const
{
	return *this / this->len();
}

inline float
Vector::dist( const Vector& rv ) const
{
	return ( *this - rv ).len();
}

inline void
Vector::copyTo( float *dest )
{
	dest[ 0 ] = v[ 0 ];
	dest[ 1 ] = v[ 1 ];
	dest[ 2 ] = v[ 2 ];
}

inline Vector& 
Vector::operator+=( 
	const Vector& rv )
{
	*this = *this + rv;
	return *this;
}

inline Vector& 
Vector::operator-=( 
	const Vector& rv )
{
	*this = *this - rv;
	return *this;
}

inline
Vector::Vector( 
	float _x,
	float _y,
	float _z )
{
	v[ 0 ] = _x;
	v[ 1 ] = _y;
	v[ 2 ] = _z;
}

inline
Vector::Vector( 
	float *arr_ptr )
{
	v[ 0 ] = arr_ptr[ 0 ];
	v[ 1 ] = arr_ptr[ 1 ];
	v[ 2 ] = arr_ptr[ 2 ];
}

inline std::ostream&
operator<<(
	std::ostream& os,
	const Vector& vec )
{
	os << "{ " << vec.v[ 0 ] << ", " << vec.v[ 1 ] << ", " << vec.v[ 2 ] << " }";
	return os;
}
