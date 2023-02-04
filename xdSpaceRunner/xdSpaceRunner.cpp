// xdSpaceRunner.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>

#include "Camera.h"
#include "Shapes.h"
#include "Vector3.h"
#include <stdio.h>
#include <conio.h>
#include <vector>
#include <windows.h>

#include "Quaternion.h"
#include "Scene.h"
#include "Transform.h"
#include <future>

using namespace xdSpace;


[[deprecated]]
void PreTestStuff()
{
	Vector3 v(0, 2, 3);
	Vector3 r = v * 4;
	r *= 0.25f;
	Vector3 w = v + r;

	float k = dot(w, v);
}


void unitTestLinearOps()
{
	Vector3 a(24, 1, 32), b(12, 3, 54);
	float k = 5.f;
	assert(AreEqual(a + b, b + a));
	assert(AreEqual((a + b) * k, b * k + a * k));
}


void unitTestProducts()
{
	Vector3 a(24, -3, 32), b(12, 3, -54), c(10, 13,-43);
	float k = .5f;
	assert(IsZero(dot(a, cross(a, b))));
	assert(IsZero(dot(b, cross(a, b))));

	assert(IsZero(cross(a, b) + cross(b, a)));

	assert(AreEqual(dot(k * a, b), dot(b, k * a)));
	assert(AreEqual(cross(a, b + 4*c), cross(a, b) + 4* cross(a, c)));
}

void unitTestRayCast()
{
	Ray r(Point3(0, 0, 0), Vector3(1, 0, 0));
	Sphere s(Point3(5, 0, 0), 3);
	Point3 hitPoint, hitNormal;
	float maxDistance = 1000.f;
	bool hit = RayCast(r, s, hitPoint, hitNormal, maxDistance);
	assert(hit);
	assert(AreEqual(hitPoint, Vector3(2, 0, 0)));
}


void unitTestQuaternions()
{
	Quaternion rotation = Quaternion::FromAngleAxis(180, Vector3(0, 1, 0));
	Vector3 point(0, 0, 1);
	assert(AreEqual(rotation.Apply(point), Vector3(0, 0, -1)));

	rotation = Quaternion::FromAngleAxis(90, Vector3(0, 1, 0));
	point = Vector3(3, 5, 6);
	assert(AreEqual(rotation.Apply(point), Vector3(6, 5, -3)));


	rotation = Quaternion::FromAngleAxis(30, Vector3(1, 1, 3));
	point = Vector3(3, 5, 6);
	Vector3 q = point;
	for (int i = 0; i < 12; ++i)
	{
		q = rotation.Apply(q);
	}
	assert(AreEqual(q, point));

	rotation = Quaternion::Identity();
	point = Vector3(4, 3, 1);
	float deg = 90.f;

	Quaternion qRot = Quaternion::FromAngleAxis(deg, point);

	for (int i = 0; i < 8; ++i)
	{
		rotation *= qRot;
	}

	assert(AreEquivalent(rotation, Quaternion::Identity()));
	assert(AreEqual(rotation, Quaternion::Identity()));

}


void unitTestTransforms()
{
	Transform transform;
	transform.rotation = Quaternion::FromAngleAxis(43.4f, Vector3(1, -3, -2));
	transform.translation = Vector3(1, 3, 4);
	transform.scale = 5;

	Vector3 point(4, 10, -13);

	Vector3 q = transform.TransformPoint(point);
	Vector3 r = transform.Inverse().TransformPoint(q);
	assert(AreEqual(point, r));

	Transform ti = transform;
	ti.Invert();
	r = ti.TransformPoint(q);
	assert(AreEqual(point, r));


	Transform tA;
	tA.rotation = Quaternion::FromAngleAxis(-13.4f, Vector3(13, 4, 0));
	tA.translation = Vector3(0, -1, 1);
	tA.scale = 0.23f;

	Transform tB = transform;

	Transform tAB = tA * tB;
	assert(AreEqual(tAB.TransformPoint(point),
		tA.TransformPoint(tB.TransformPoint(point))));

}


const char* IntensityToCStr(float intensity)
{
	switch (int (round(intensity * 8)))
	{
	case 0: return "  ";
	case 1: return "' ";
	case 2: return "_ ";
	case 3: return "+ ";
	case 4: return "* ";
	case 5: return " #";
	case 6: return "'#";
	case 7: return "*#";
	case 8: return "–‚";
	default: return "&";
	}
}

std::chrono::time_point<std::chrono::system_clock> start;


float getCurrentTime()
{
	auto end = std::chrono::system_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 100.f;
}

Versor3 lightDir;


const char* Lighting(const Versor3& normal)
{
	//lambert lighting
	float diffuse = dot(normal, lightDir);
	if (diffuse < 0) diffuse = 0;
	return IntensityToCStr(diffuse);
}

void RayCastSphereFromCamera()
{
	float time = getCurrentTime();

	Camera camera(2.f, 300, 300);
	Sphere sphere(Point3(0, std::sin(time * 0.3f) * 0.5f, 6.f), 2.f);
	Plane plane(Point3(0, -1, 0), Point3(0, 1, 0));
	std::string outStream;
	lightDir = Versor3(std::cos(time * 0.5f), 1.f, std::sin(time * 0.2f) -2).Normalized();

	for(int y = 0; y < camera.pixelCountY; ++y)
	{
		for(int x = 0; x < camera.pixelCountX; ++x)
		{
			Point3 hitPos, hitNormal;
			float maxDistance = 1000.f;
			RayCast(camera.PrimaryRay(x, y), sphere, hitPos, hitNormal, maxDistance);
			RayCast(camera.PrimaryRay(x, y), plane, hitPos, hitNormal, maxDistance);

			outStream += Lighting(hitNormal);
		}
		outStream += "\n";
	}
	//system("pause");

	std::cout << outStream;
}

void RayCastGameObjects(const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const Camera& camera)
{
	float time = getCurrentTime();

	Plane plane(Point3(0, -1, 0), Point3(0, 1, 0));
	std::string outStream;
	lightDir = Versor3(std::cos(time * 0.5f), 1.f, std::sin(time * 0.2f) - 2).Normalized();

	for (int y = 0; y < camera.pixelCountY; ++y)
	{
		for (int x = 0; x < camera.pixelCountX; ++x)
		{
			Point3 hitPos, hitNormal;
			float maxDistance = 1000.f;

			for (const Sphere& sphere : spheres)
			{
				RayCast(camera.PrimaryRay(x, y), sphere, hitPos, hitNormal, maxDistance);
			}
			for (const Plane& plane : planes)
			{
				RayCast(camera.PrimaryRay(x, y), plane, hitPos, hitNormal, maxDistance);
			}


			//RayCast(camera.PrimaryRay(x, y), plane, hitPos, hitNormal, maxDistance);

			outStream += Lighting(hitNormal);
		}
		outStream += "\n";
	}
	//system("pause");

	std::cout << outStream;
}


int nonBlockingGetChar()
{
	if (_kbhit()) {
		return _getch();
	}
	else {
		return -1;
	}
}

int nonBlockingGetCharTask()
{	
	return nonBlockingGetChar();
}

void handleInput(const int keyInput, Scene& scene, int& objToControl, bool& viewMode, bool& enablePlane)
{
	switch (keyInput)
	{
	case 72: //arrow up
		++objToControl;
		if (objToControl >= scene.objs.size()) objToControl = 0;
		break;
	case 80: //arrow down
		--objToControl;
		if (objToControl < 0) objToControl = scene.objs.size() - 1;
		break;
	case 9:
		enablePlane = !enablePlane;
		break;
	case ' ':
		viewMode = !viewMode;
		break;
	default:

		if (keyInput >= 48 && keyInput <= 57)
		{
			objToControl = keyInput - 48;
			if (objToControl >= scene.objs.size()) objToControl = scene.objs.size() - 1;
		}
		else
		{
			Transform temp;
			switch (keyInput)
			{
			case 'w': temp.translation = Forward * 0.3; break; //move forward
			case 's': temp.translation = Forward * -0.3; break; //move backward
			case 'A': temp.translation = Right * -0.3; break; //move left
			case 'D': temp.translation = Right * 0.3; break; //move right
			case 'R': temp.translation = Up * 0.3; break; //move up
			case 'F': temp.translation = Up * -0.3; break; //move down
			case 'a': temp.rotation = Quaternion::FromAngleAxis(-2, Up); break; //rotate left
			case 'd': temp.rotation = Quaternion::FromAngleAxis(2, Up); break; //rotate right
			case 'r': temp.rotation = Quaternion::FromAngleAxis(-2, Right); break; //rotate up
			case 'f': temp.rotation = Quaternion::FromAngleAxis(2, Right); break; //rotate down
			}
			scene.objs[objToControl].transform = scene.objs[objToControl].transform * temp;
		}
	}


}

void fun(int spheresToPopulate, int pixelsX, int pixelsY)
{
	auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = 0;
	coord.Y = 0;
	std::srand(std::time(nullptr));

	Scene scene;
	scene.planes.push_back(WorldPlane());
	scene.Populate(spheresToPopulate);
	std::vector<Sphere> spheres;
	std::vector<Plane> planes;

	Camera camera(2.f, pixelsX, pixelsY);

	int objToControl = 0;
	bool viewFromObj = false;
	bool enablePlane = true;

	while (true)
	{
		std::future<int> getCharHandle{ std::async(std::launch::async, nonBlockingGetCharTask) };

		
		// Finally, call the SetConsoleCursorPosition function.
		SetConsoleCursorPosition(hConsole, coord);
		//system("cls");
		//RayCastSphereFromCamera();
		viewFromObj ? scene.ToObjectLocal(spheres, objToControl) : scene.ToWorld(spheres);
		if (enablePlane)
			viewFromObj ? scene.ToObjectLocal(planes, objToControl) : scene.ToWorld(planes);
		else
			planes.clear();

		RayCastGameObjects(spheres, planes, camera);
		int userInput{ getCharHandle.get() };
		if (userInput > -1)
		{
			handleInput(userInput, scene, objToControl, viewFromObj, enablePlane);
		}
	}
}



int main()
{

	unitTestLinearOps();
	unitTestProducts();
	unitTestRayCast();

	unitTestQuaternions();
	unitTestTransforms();

	//return 1;
	std::cout << "Controls: \n - arrow up/down to cycle target to control." <<
		"\n - 0-9 to control the first 10 targets." <<
		"\n - space to switch between 1st person and world view. (world is default)." <<
		"\n - w/s to move forward/backward. A/D to move left/right. R/F to move up/down." <<
		"\n - a/d to rotate left/right. r/t to rotate up/down." <<
		"\n - tab to enable/disable the plane." <<
		"\n\nzoom out of the console and make the window larger, the resolution might be set quite high!\n";
	system("pause");
	start = std::chrono::system_clock::now();

	fun(26, 100, 100);
}