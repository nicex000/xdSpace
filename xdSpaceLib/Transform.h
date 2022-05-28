#pragma once
#include "Vector3.h"
#include "Quaternion.h"
namespace xdSpace
{
	class Transform
	{
	public:

		float scale;
		Vector3 translation;
		Quaternion rotation;

		Transform(): scale(1.f), translation(0,0,0), rotation(Quaternion::Identity()) {}

		Vector3 TransformPoint(const Vector3& point) const
		{
			return rotation.Apply(point * scale) + translation;
		}

		Vector3 TransformVector(const Vector3& point) const
		{
			return rotation.Apply(point * scale);
		}

		Vector3 TransformVersor(const Vector3& point) const
		{
			return rotation.Apply(point);
		}
		float TransformScale(float p) const
		{
			return p * scale;
		}

		Transform Inverse() const
		{
			Transform t;
			t.scale = 1 / scale;
			t.rotation = rotation.Conjugated();
			t.translation = t.rotation.Apply(-translation * t.scale);

			return t;
		}

		void Invert()
		{
			scale = 1 / scale;
			rotation.Conjugate();
			translation = rotation.Apply(-translation * scale);
			// alternatively translation = applyToVector(-translation);
		}

		//first b then a (self)
		Transform operator* (const Transform &b) const
		{
			Transform t;
			t.rotation = rotation * b.rotation;
			t.scale = scale * b.scale;
			t.translation = TransformVector(b.translation) + translation;
			return t;
		}
	};

}

