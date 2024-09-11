#include <iostream>
#include <random>
#include "sphere.h"
#include "camera.h"

double randomF64()
{
	return rand() / (RAND_MAX + 1.0);
}

vec3 randomInUnitSphere()
{
	vec3 p;
	do
	{
		p = 2.0f * vec3(randomF64(), randomF64(), randomF64()) - vec3(1,1,1);
	} while (p.squaredLength() >= 1.0f);
	return p;
}

vec3 color(const ray &r, hitable *world, int depth)
{
	hitRecord rec;
	if (depth <= 0)
	{
		return vec3(0, 0, 0);
	}
	if (world->hit(r, 0.001, MAXFLOAT, rec))
	{
		vec3 target = rec.p + rec.normal + randomInUnitSphere();
		return 0.5f * color(ray(rec.p, target - rec.p), world, depth - 1);
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
	int nx = 1200;
	int ny = 600;
	int ns = 100;
	std::cout << "P3\n"
			  << nx << " " << ny << "\n255\n";

	Camera camera;

	hitable *sphereList[]=
	{
		new Sphere(vec3(0, 0, -1), 0.5f),
		new Sphere(vec3(0, -100.5f, -1), 100)
	};
	hitable *world = new HitableList(sphereList, sizeof(sphereList) / sizeof(sphereList[0]));
	for (int j = ny - 1; j >= 0; j--) // 縦
	{
		for (int i = 0; i < nx; i++) // 横
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + drand48()) / float(nx - 1);
				float v = float(j + drand48()) / float(ny - 1);

				ray r = camera.getRay(u,v);
				col += color(r, world, 50);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]),sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}