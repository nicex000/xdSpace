#pragma once
#include <cassert>
#include <cmath>

namespace xdSpace
{
	typedef float Scalar;

	struct Vector3
	{
		Scalar x, y, z;

		Vector3() : x(0), y(0), z(0)
		{}

		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
		{}

		Vector3 operator*(Scalar k) const { return { k * x, k * y, k * z }; }
		void operator*=(Scalar k) { x *= k; y *= k; z *= k; }

		Vector3 operator/(Scalar k) const { return { k / x, k / y, k / z }; }
		void operator/=(Scalar k) { x /= k; y /= k; z /= k; }

		Vector3 operator+(const Vector3& v) const { return { x + v.x, y + v.y, z + v.z }; }
		void operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; }

		Vector3 operator-(const Vector3& v) const { return { x - v.x, y - v.y, z - v.z }; }
		void operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; }

		Vector3 operator-() const { return {-x, -y, -z}; }
		Vector3 operator+() const { return {x, y, z}; }

		Scalar operator[](int i) const {
			switch (i)
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: assert(0); return 0;
			}
		}
		Scalar& operator[](int i) {
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: assert(0); return x;
			}
		}


		bool operator==(const Vector3& b) const { return x == b.x && y == b.y && z == b.z; }


		Scalar SquareMagnitude() const { return x * x + y * y + z * z; }
		Scalar Magnitude() const { return sqrt(SquareMagnitude()); }
		void Normalize() { *this *= 1 / Magnitude(); }
		Vector3 Normalized() const { return *this * (1 / Magnitude()); }
	};

	constexpr Scalar TOLERANCE = 1e-5f;
	inline bool isZero(const Scalar& a, const Scalar tolerance = TOLERANCE)
	{
		return std::abs(a) < tolerance;
	}
	inline bool areEqual(const Scalar& a, const Scalar& b, const Scalar tolerance = TOLERANCE)
	{
		return std::abs(a - b) < tolerance;
	}
	inline bool areEqual(const Vector3& a, const Vector3& b, const Scalar tolerance = TOLERANCE)
	{
		return areEqual(a.x, b.x, tolerance) &&
			areEqual(a.y, b.y, tolerance) &&
			areEqual(a.z, b.z, tolerance);
	}


	inline bool isZero(const Vector3& a, const Scalar tolerance = TOLERANCE)
	{
		return isZero(a.x, tolerance) &&
			isZero(a.y, tolerance) &&
			isZero(a.z, tolerance);
	}

	inline Scalar dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
	inline Vector3 cross(const Vector3& a, const Vector3& b) { return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	}; }


	inline Vector3 operator*(Scalar k, const Vector3& a) { return a*k; }

	typedef Vector3 Point3;
	typedef Vector3 Versor3;

}
