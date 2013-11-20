#pragma once

#include <math.h>

namespace util
{

	inline float 
	randInRange( 
		float min, 
		float max );

	template < class T > T
	clamp( 
		const T& t, 
		const T& min,
		const T& max );

}

inline float
util::randInRange(
	float min,
	float max )
{
	return min + ( max - min ) * 
		( static_cast< float >( rand() ) / 
		  static_cast< float >( RAND_MAX ) );
}


template < class T > T 
util::clamp( 
	const T& t, 
	const T& min, 
	const T& max )
{
	return t < min ? min : t > max ? max : t;
}
