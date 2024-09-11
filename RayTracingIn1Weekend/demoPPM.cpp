#include <iostream>
#include "sphere.h"

vec3 color(const ray &r, hitable *world)
{
	hitRecord rec;
	if (world->hit(r, 0.0, MAXFLOAT, rec))
	{
		return 0.5f * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else
	{
		vec3 unitDirection = normalize(r.direction());
		float t = 0.5f * (unitDirection.y() + 1.0f);
		return vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + vec3(0.5f, 0.7f, 1.0f) * t;
	}
}

int main()
{
	int nx = 600;
	int ny = 300;
	std::cout << "P3\n"
			  << nx << " " << ny << "\n255\n";

	vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0, 0.0, 0.0);

	hitable *sphereList[2];
	sphereList[0] = new Sphere(vec3(0, 0, -1), 0.5f);
	sphereList[1] = new Sphere(vec3(0, -100.5f, -1), 100);
	hitable* world = new HitableList(sphereList, 2);
	for (int j = ny - 1; j >= 0; j--) // 縦
	{
		for (int i = 0; i < nx; i++) // 横
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			vec3 col = color(r, world);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}