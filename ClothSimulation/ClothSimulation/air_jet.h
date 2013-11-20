#pragma once

#include "vector.h"

namespace force
{
	class AirJet
	{
	public:

		//	functions
		//
		float
		getForce() const;

		float
		getCosCutoff() const;

		float
		getDistCutoff() const;

		const Vector&
		getPosition() const;

		void
		setPosition(
			const Vector& p );

		const Vector&
		getDirection() const;

		void
		setDirection(
			const Vector& d );

		//	ctors
		//
		AirJet(
			float force,
			const Vector& dir,
			const Vector& pos,
			float cos_cutoff,
			float dist_cutoff );

		float phase_delta;

	private:

		//	variables
		//
		Vector
			dir_,
			pos_;

		float
			force_,
			cos_cutoff_,
			dist_cutoff_;
	};

	inline float
	AirJet::getForce() const
	{
		return force_;
	}

	inline float
	AirJet::getCosCutoff() const
	{
		return cos_cutoff_;
	}

	inline float
	AirJet::getDistCutoff() const
	{
		return dist_cutoff_;
	}

	inline const Vector&
	AirJet::getPosition() const
	{
		return pos_;
	}

	inline void
	AirJet::setPosition(
		const Vector& p )
	{
		pos_ = p;
	}

	inline void
	AirJet::setDirection(
		const Vector& d )
	{
		dir_ = d;
	}

	inline const Vector&
	AirJet::getDirection() const
	{
		return dir_;
	}

	AirJet::AirJet(
		float force,
		const Vector& dir,
		const Vector& pos,
		float cos_cutoff,
		float dist_cutoff )
		:	force_( force )
		,	dir_( dir )
		,	pos_( pos )
		,	cos_cutoff_( cos_cutoff )
		,	dist_cutoff_( dist_cutoff )
	{}
}