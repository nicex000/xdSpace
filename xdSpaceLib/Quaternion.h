#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "Vector3.h"

namespace xdSpace
{
	class Quaternion
	{
	public:
		Vector3 imaginary;
		float real;

		Quaternion(Vector3 _imaginary, float _real) : imaginary(_imaginary), real(_real) {}

		Vector3 Apply(const Vector3& position) const
		{
	 		Quaternion q = FromVector3(position);
	 		q = (*this) * q * this->Conjugated();
	
	 		return q.imaginary;
		}

		void Conjugate() {
			imaginary = -imaginary;
		}

		Quaternion Conjugated() const {
			return {-imaginary, real};
		}

		Quaternion operator* (const Quaternion& b) const
		{
			return {
				cross(imaginary, b.imaginary) + imaginary * b.real + real * b.imaginary,
				-dot(imaginary, b.imaginary) + real * b.real
			};
		}

		void operator*= (const Quaternion& b)
		{
			Quaternion q = (*this) * b;
			imaginary = q.imaginary;
			real = q.real;
		}

		Quaternion operator- () const
		{
			return { -imaginary, -real };
		}
		
		static Quaternion FromVector3(Vector3 v) { return {v, 0}; }
		static Quaternion FromAngleAxis(float deg, Vector3 axis)
		{
			float rad = deg * M_PI / 180;
			float s = std::sin(rad/2);
			float c = std::cos(rad/2);
			return {axis.Normalized() * s, c};
		}

		static Quaternion Identity()
		{
			return { Vector3(0,0,0),1 };
		}

	};

	inline bool AreEqual(const Quaternion& a, const Quaternion& b)
	{
		return AreEqual(a.imaginary, b.imaginary) && AreEqual(a.real, b.real);
	}

	inline bool AreEquivalent(const Quaternion& a, const Quaternion& b)
	{
		return AreEqual(a, b) || AreEqual(a, -b);
	}

}
