#pragma once

#include <vector>
#include "vector.h"

class Vertex;

struct Triangle
{
	//	variables
	Vertex *v[3];

	Vector norm;

	//	functions
	Vertex*&
	operator[]( 
		int i );

	//	ctors
	Triangle(
		Vertex *v1 = 0,
		Vertex *v2 = 0,
		Vertex *v3 = 0 );
};

inline Vertex*&
Triangle::operator[](
	int i )
{
	return v[ i ];
}

inline
Triangle::Triangle(
	Vertex *v1,
	Vertex *v2,
	Vertex *v3 )
{
	v[ 0 ] = v1;
	v[ 1 ] = v2;
	v[ 2 ] = v3;
}