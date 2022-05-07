#pragma once
#include "Vector3.h"

namespace xdSpace
{
	struct Ray
	{
		Ray(){};
		Ray(Point3 _p, Vector3 _d) : pos(_p), dir(_d.Normalized()) {};
		Point3 pos;
		Versor3 dir;
	};

	struct Sphere
	{
		Sphere(Point3 _c, Scalar _r) : center(_c), radius(_r) {};
		Point3 center;
		Scalar radius;
	};

	struct Plane
	{
		Plane(Point3 _p, Versor3 _n) : pos(_p), normal(_n) {};
		Point3 pos;
		Versor3 normal;
	};

	bool rayCast(Ray ray, Sphere sphere, Point3& hitPos, Point3& hitNormal, float& maxDistance)
	{

		//a*k^2 + b*k + c = 0
		Scalar a = 1.f;
		Scalar b = 2.f * dot(ray.dir, ray.pos - sphere.center);
		Scalar c = (ray.pos - sphere.center).SquareMagnitude() - sphere.radius * sphere.radius;

		Scalar delta = b*b - 4*a*c;
		if (delta < 0) return false; //ray missed the sphere

		Scalar distance = (-b - sqrt(delta)) / (2 * a);

		if (distance < 0) return false;
		if (distance > maxDistance) return false;
		maxDistance = distance;

		hitPos = ray.pos + distance * ray.dir;
		hitNormal = (hitPos - sphere.center).Normalized();
		return true;
	}

	bool rayCast(Ray ray, Plane plane, Point3& hitPos, Point3& hitNormal, float& maxDistance)
	{
		Scalar dn = dot(ray.dir, plane.normal);
		if (dn == 0.f) return false;

		Scalar distance = dot(plane.pos - ray.pos, plane.normal) / dn;
		if (distance < 0.f) return false;
		if (distance > maxDistance) return false;
		maxDistance = distance;

		hitPos = ray.pos + distance * ray.dir;
		hitNormal = plane.normal;
		return true;
	}

	
}
