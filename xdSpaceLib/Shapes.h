#pragma once
#include "Transform.h"
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
		Sphere(Point3 _c, float _r) : center(_c), radius(_r) {};
		Point3 center;
		float radius;
	};

	struct Plane
	{
		Plane(Point3 _p, Versor3 _n) : pos(_p), normal(_n) {};
		Point3 pos;
		Versor3 normal;
	};

	Sphere Apply(const Transform& a, const Sphere& s)
	{
		return Sphere(a.TransformPoint(s.center), a.TransformScale(s.radius));
	}

	Plane Apply(const Transform& a, const Plane& p)
	{
		return Plane(a.TransformPoint(p.pos), a.TransformVersor(p.normal));
	}

	bool RayCast(Ray ray, Sphere sphere, Point3& hitPos, Point3& hitNormal, float& maxDistance)
	{

		//a*k^2 + b*k + c = 0
		float a = 1.f;
		float b = 2.f * dot(ray.dir, ray.pos - sphere.center);
		float c = (ray.pos - sphere.center).SquareMagnitude() - sphere.radius * sphere.radius;

		float delta = b*b - 4*a*c;
		if (delta < 0) return false; //ray missed the sphere

		float distance = (-b - sqrt(delta)) / (2 * a);

		if (distance < 0) return false;
		if (distance > maxDistance) return false;
		maxDistance = distance;

		hitPos = ray.pos + distance * ray.dir;
		hitNormal = (hitPos - sphere.center).Normalized();
		return true;
	}

	bool RayCast(Ray ray, Plane plane, Point3& hitPos, Point3& hitNormal, float& maxDistance)
	{
		float dn = dot(ray.dir, plane.normal);
		if (dn == 0.f) return false;

		float distance = dot(plane.pos - ray.pos, plane.normal) / dn;
		if (distance < 0.f) return false;
		if (distance > maxDistance) return false;
		maxDistance = distance;

		hitPos = ray.pos + distance * ray.dir;
		hitNormal = plane.normal;
		return true;
	}

	
}
