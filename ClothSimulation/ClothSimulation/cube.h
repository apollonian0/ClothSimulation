#pragma once

#include "collidable.h"

namespace static_geometry
{
	class Cube
		:	public collision::Collidable
	{
	public:

		//	functions
		//
		float
		getLength() const;

		//	ctors
		//
		Cube(
			float length );
	private:

		//	variables
		//
		float length_;
	};

	inline float
	Cube::getLength() const
	{
		return length_;
	}

	inline 
	Cube::Cube(
		float length )
		:	length_( length )
	{}
}