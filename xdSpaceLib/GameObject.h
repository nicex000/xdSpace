#pragma once
#include "Shapes.h"
#include "Transform.h"

namespace xdSpace
{
	class GameObject
	{
	public:
		Transform transform;
		Sphere body, nose;

		GameObject() :
			transform(),
			body(Vector3(0, 0, 0), 1),
			nose(Vector3(0, 0.8f, -0.6f), 0.3f)
		{}
	};

	class WorldPlane
	{
	public:
		Transform transform;
		Plane plane;

		WorldPlane() :
			transform(),
			plane(Point3(0, -1, 0), Point3(0, 1, 0))
		{}
	};
}
