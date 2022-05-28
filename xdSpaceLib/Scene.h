#pragma once
#include <vector>

#include "GameObject.h"

namespace xdSpace
{
	class Scene
	{
	public:
		std::vector<GameObject> objs;

		void Populate(int n)
		{
			for (int i = 0; i < n; ++i)
			{
				GameObject newObj;
				newObj.transform.translation = Vector3::Random(-10, 10);
				newObj.transform.translation.z += 16;
				objs.push_back(newObj);
			}
		}

		void ToWorld(std::vector<Sphere>& res) const
		{
			res.clear();
			for (const GameObject& go : objs)
			{
				res.push_back(Apply(go.transform, go.body));
				res.push_back(Apply(go.transform, go.nose));
			}
		}
	};
}
