#include "sphere.h"
#include "camera.h"
#include "util.h"
#include <omp.h>

vec3 color(const ray &r, hitable *world, int depth)
{
	hitRecord rec;
	if (world->hit(r, 0.001, MAXFLOAT, rec))
	{
		ray scattered;
		vec3 attenuation;

		if (depth >= 0 && rec.pMaterial->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth - 1);
		}
		else
		{
			return vec3(0, 0, 0);
		}
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
	int resolutionX = 600;
	int resolutionY = 300;
	int ns = 30;
	std::cout << "P3\n"
			  << resolutionX << " " << resolutionY << "\n255\n";

	Camera camera;

	hitable *sphereList[]=
	{
		new Sphere(vec3(0, 0, -1.0), 0.5f, new Lambertian(vec3(0.1f, 0.2, 0.5f))),
		new Sphere(vec3(0, -100.5f, -1), 100, new Metal(vec3(0.8f, 0.8, 0.0f))),
		new Sphere(vec3(1.5, 0, -1), 0.5, new Metal(vec3(0.8f, 0.6, 0.2f), 0.0f)),
		new Sphere(vec3(-1.0, 0, -1), 0.5, new Dielectric(1.5)),
		new Sphere(vec3(-1.0, 0, -1), -0.45, new Dielectric(1.5)),
		new Sphere(vec3(0, 2, 1), 0.5, new Metal(vec3(1.0f, 1.0, 1.0f), 0.1f))
	};
	hitable *world = new HitableList(sphereList, sizeof(sphereList) / sizeof(sphereList[0]));

	vec3* resultColor = new vec3[resolutionX * resolutionY];
	#pragma omp parallel for
	for (int index = 0; index < (resolutionX * resolutionY); index++)
	{
		int j = index / resolutionX;
		int i = index % resolutionX;
		vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + drand48()) / float(resolutionX - 1);
				float v = float(j + drand48()) / float(resolutionY - 1);

				ray r = camera.getRay(u,v);
				col += color(r, world, 30);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]),sqrt(col[1]), sqrt(col[2]));
			resultColor[index].e[0] = int(255.99 * col[0]);
			resultColor[index].e[1] = int(255.99 * col[1]);
			resultColor[index].e[2] = int(255.99 * col[2]);
	}
	for (int j = resolutionY - 1; j >= 0; j--) // 縦
	{
		for (int i = 0; i < resolutionX; i++) // 横
		{
			const int index = j * resolutionX + i;
			std::cout << resultColor[index].r() << " " << resultColor[index].g() << " " << resultColor[index].b() << "\n";
		}
	}
}