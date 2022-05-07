#pragma once
#include "Shapes.h"
#include "Vector3.h"
namespace xdSpace
{
	class Camera
	{
	public:
		Camera() {};
		Camera(Scalar _f, int _pX, int _pY): focalDistance(_f), pixelCountX(_pX), pixelCountY(_pY) {};

		const Point3 eye = Point3(0,0,0); // forcing the eye to be in origin

		int pixelCountX, pixelCountY;

		Scalar focalDistance;

		Ray primaryRay(int pixelX, int pixelY) const
		{
			Ray r;
			r.pos = eye;

			Scalar clipX = 2.f * pixelX / pixelCountX  - 1.f;
			Scalar clipY = -2.f * pixelY / pixelCountY + 1.f;

			r.dir = Vector3(clipX, clipY, focalDistance).Normalized();
			return r;
		}
	};
}

