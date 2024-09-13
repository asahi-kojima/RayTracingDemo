#include "sphere.h"
#include "camera.h"
#include "util.h"
#include <omp.h>
#include <atomic>
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
	srand(static_cast<unsigned int>(time(NULL)));

	constexpr int BaseResolution = 2000;
	int resolutionX = 2 * BaseResolution;
	int resolutionY = BaseResolution;
	int ns = 50;
	std::cout << "P3\n"
			  << resolutionX << " " << resolutionY << "\n255\n";

	float zoom = 7;
	Camera camera(vec3(3, 1, 2), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(resolutionX) / float(resolutionY), zoom);

	hitable *sphereListBase[] =
	{
		new Sphere(vec3(0, -300.5f, -1.0 - 0.3), 300, new Metal(vec3(0.8f, 0.8, 0.0f))),

		new Sphere(vec3(1.3, 0, -0.8 - 0.3), 0.50, new Dielectric(2.0)),
		new Sphere(vec3(1.3, 0, -0.8 - 0.3), -0.45, new Dielectric(2.0)),

		new Sphere(vec3(0.0, 0, -1.0 - 0.3), 0.5f, new Metal(vec3(0.1f, 0.2, 0.5f))),

		new Sphere(vec3(-1.3, 0, -1.2 - 0.3), 0.50, new Dielectric(1.2)),
		new Sphere(vec3(-1.3, 0, -1.2 - 0.3), 0.45, new Dielectric(1.2)),
		new Sphere(vec3(-1.3, 0, -1.2 - 0.3), 0.40, new Dielectric(1.2)),
		new Sphere(vec3(-1.3, 0, -1.2 - 0.3), 0.35, new Dielectric(1.2)),
		new Sphere(vec3(-1.3, 0, -1.2 - 0.3), 0.30, new Dielectric(1.2)),
		new Sphere(vec3(-1.3, 0, -1.2 - 0.3), 0.25, new Dielectric(1.2)),
		new Sphere(vec3(-1.3, 0, -1.2 - 0.3), 0.20, new Dielectric(1.2)),
		new Sphere(vec3(-1.3, 0, -1.2 - 0.3), 0.15, new Dielectric(1.2)),
		new Sphere(vec3(-1.3, 0, -1.2 - 0.3), 0.10, new Dielectric(1.2)),
		new Sphere(vec3(-1.3, 0, -1.2 - 0.3), 0.05, new Dielectric(1.2)),

		new Sphere(vec3(-2.6, 0, -1.4 - 0.3), 0.5, new Metal(vec3(1.0f, 0.3, 0.0f), 0.0f))
	};

	hitable* sphereList[200];
	for (int i = 0 ; i < sizeof(sphereList) / sizeof(sphereList[0]); i++)
	{
		if(i < sizeof(sphereListBase) / sizeof(sphereListBase[0]))
		{
			sphereList[i] = sphereListBase[i];
		}
		else
		{
			Material* pMaterial = nullptr;
			if (i % 3 == 0)
			{
				pMaterial = new Metal(vec3(randomF64(), randomF64(), randomF64()));
			}
			else if (i % 3 == 1)
			{
				pMaterial = new Lambertian(vec3(randomF64(), randomF64(), randomF64()));
			}
			else
			{
				pMaterial = new Dielectric(1 + randomF64() * 2);
			}
			sphereList[i] = new Sphere(vec3(srandomF64(), 0, srandomF64()) * 5 - 1 + vec3(-3, 1 - 0.4, -2) ,0.1, pMaterial);
		}
	}

	hitable *world = new HitableList(sphereList, sizeof(sphereList) / sizeof(sphereList[0]));

	vec3 *resultColor = new vec3[resolutionX * resolutionY * ns];


	std::atomic<int> progressCounter(0);

#pragma omp parallel for
	for (int index = 0; index < (resolutionX * resolutionY * ns); index++)
	{
		int s = index % ns;
		int j = (index / ns) / resolutionX;
		int i = (index / ns) % resolutionX;

		float u = (float(i) + (2 * drand48() - 1) * 0.5) / float(resolutionX - 1);
		float v = (float(j) + (2 * drand48() - 1) * 0.5) / float(resolutionY - 1);

		ray r = camera.getRay(u, v);
		resultColor[index] = color(r, world, 30);

		if (s == 0)
		{
			progressCounter+=1;
			std::cerr << (float)progressCounter * 100 / (resolutionX * resolutionY) << std::endl;
		}
	}

	for (int index = 0; index < (resolutionX * resolutionY); index++)
	{
		int j = resolutionY - 1 - (index / resolutionX);
		int i = index % resolutionX;
		vec3 col(0, 0, 0);
		for (int s = 0; s < ns; s++)
		{
			col += resultColor[(j * resolutionX + i) * ns + s];
		}
		col /= ns;
		col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
		col.e[0] = int(255.99 * col[0]);
		col.e[1] = int(255.99 * col[1]);
		col.e[2] = int(255.99 * col[2]);
		std::cout << col.r() << " " << col.g() << " " << col.b() << "\n";
	}
}