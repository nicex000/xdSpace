#pragma once
#include <cassert>
#include <cmath>
#include <iostream>

namespace xdSpace
{
	inline float randomFloatInRange(float min, float max)
	{
		return min + (std::rand() % 1001 / 1000.f) * (max - min);
	}

	struct Vector3
	{
		float x, y, z;

		Vector3() : x(0), y(0), z(0)
		{}

		Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
		{}

		Vector3 operator*(float k) const { return { k * x, k * y, k * z }; }
		void operator*=(float k) { x *= k; y *= k; z *= k; }

		Vector3 operator/(float k) const { return { k / x, k / y, k / z }; }
		void operator/=(float k) { x /= k; y /= k; z /= k; }

		Vector3 operator+(const Vector3& v) const { return { x + v.x, y + v.y, z + v.z }; }
		void operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; }

		Vector3 operator-(const Vector3& v) const { return { x - v.x, y - v.y, z - v.z }; }
		void operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; }

		Vector3 operator-() const { return {-x, -y, -z}; }
		Vector3 operator+() const { return {x, y, z}; }

		float operator[](int i) const {
			switch (i)
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: assert(0); return 0;
			}
		}
		float& operator[](int i) {
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: assert(0); return x;
			}
		}


		bool operator==(const Vector3& b) const { return x == b.x && y == b.y && z == b.z; }


		float SquareMagnitude() const { return x * x + y * y + z * z; }
		float Magnitude() const { return sqrt(SquareMagnitude()); }
		void Normalize() { *this *= 1 / Magnitude(); }
		Vector3 Normalized() const { return *this * (1 / Magnitude()); }
		static Vector3 Random(float min, float max)
		{
			return {
				randomFloatInRange(min, max), 
				randomFloatInRange(min, max), 
				randomFloatInRange(min, max)
			};
		}
	};

	constexpr float TOLERANCE = 1e-5f;
	inline bool IsZero(const float& a, const float tolerance = TOLERANCE)
	{
		return std::abs(a) < tolerance;
	}
	inline bool AreEqual(const float& a, const float& b, const float tolerance = TOLERANCE)
	{
		return std::abs(a - b) < tolerance;
	}
	inline bool AreEqual(const Vector3& a, const Vector3& b, const float tolerance = TOLERANCE)
	{
		return AreEqual(a.x, b.x, tolerance) &&
			AreEqual(a.y, b.y, tolerance) &&
			AreEqual(a.z, b.z, tolerance);
	}


	inline bool IsZero(const Vector3& a, const float tolerance = TOLERANCE)
	{
		return IsZero(a.x, tolerance) &&
			IsZero(a.y, tolerance) &&
			IsZero(a.z, tolerance);
	}

	inline float dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
	inline Vector3 cross(const Vector3& a, const Vector3& b) { return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	}; }


	inline Vector3 operator*(float k, const Vector3& a) { return a*k; }

	typedef Vector3 Point3;
	typedef Vector3 Versor3;

}
