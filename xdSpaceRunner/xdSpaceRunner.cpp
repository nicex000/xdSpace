// xdSpaceRunner.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <chrono>
#include <iostream>

#include "Camera.h"
#include "Shapes.h"
#include "Vector3.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>

using namespace xdSpace;


[[deprecated]]
void preTestStuff()
{
	Vector3 v(0, 2, 3);
	Vector3 r = v * 4;
	r *= 0.25f;
	Vector3 w = v + r;

	Scalar k = dot(w, v);
}


void unitTestLinearOps()
{
	Vector3 a(24, 1, 32), b(12, 3, 54);
	Scalar k = 5.f;
	assert(areEqual(a + b, b + a));
	assert(areEqual((a + b) * k, b * k + a * k));
}


void unitTestProducts()
{
	Vector3 a(24, -3, 32), b(12, 3, -54), c(10, 13,-43);
	Scalar k = .5f;
	assert(isZero(dot(a, cross(a, b))));
	assert(isZero(dot(b, cross(a, b))));

	assert(isZero(cross(a, b) + cross(b, a)));

	assert(areEqual(dot(k * a, b), dot(b, k * a)));
	assert(areEqual(cross(a, b + 4*c), cross(a, b) + 4* cross(a, c)));
}

void unitTestRayCast()
{
	Ray r(Point3(0, 0, 0), Vector3(1, 0, 0));
	Sphere s(Point3(5, 0, 0), 3);
	Point3 hitPoint, hitNormal;
	Scalar maxDistance = 1000.f;
	bool hit = rayCast(r, s, hitPoint, hitNormal, maxDistance);
	assert(hit);
	assert(areEqual(hitPoint, Vector3(2, 0, 0)));
}

const char* intensityToCStr(Scalar intensity)
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


const char* lighting(const Versor3& normal)
{
	//lambert lighting
	Scalar diffuse = dot(normal, lightDir);
	if (diffuse < 0) diffuse = 0;
	return intensityToCStr(diffuse);
}

void rayCastSphereFromCamera()
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
			Scalar maxDistance = 1000.f;
			rayCast(camera.primaryRay(x, y), sphere, hitPos, hitNormal, maxDistance);
			rayCast(camera.primaryRay(x, y), plane, hitPos, hitNormal, maxDistance);

			outStream += lighting(hitNormal);
		}
		outStream += "\n";
	}
	//system("pause");

	std::cout << outStream;
}
int main()
{
	system("pause");

	start = std::chrono::system_clock::now();
	unitTestLinearOps();
	unitTestProducts();
	unitTestRayCast();

	auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = 0;
	coord.Y = 0;
	while(1)
	{

		// Finally, call the SetConsoleCursorPosition function.
		SetConsoleCursorPosition(hConsole, coord);
		//system("cls");
		rayCastSphereFromCamera();
	}
	
}