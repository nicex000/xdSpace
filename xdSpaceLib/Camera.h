#pragma once
#include "Shapes.h"
#include "Vector3.h"
namespace xdSpace
{
	class Camera
	{
	public:
		Camera() {};
		Camera(float _f, int _pX, int _pY): focalDistance(_f), pixelCountX(_pX), pixelCountY(_pY) {};

		const Point3 eye = Point3(0,0,0); // forcing the eye to be in origin

		int pixelCountX, pixelCountY;

		float focalDistance;

		Ray PrimaryRay(int pixelX, int pixelY) const
		{
			Ray r;
			r.pos = eye;

			float clipX = 2.f * pixelX / pixelCountX  - 1.f;
			float clipY = -2.f * pixelY / pixelCountY + 1.f;

			r.dir = Vector3(clipX, clipY, focalDistance).Normalized();
			return r;
		}
	};
}

