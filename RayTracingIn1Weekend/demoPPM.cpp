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

	constexpr int BaseResolution = 2;
	int resolutionX = 1920 * BaseResolution;
	int resolutionY = 1080 * BaseResolution;
	int ns = 20;
	std::cout << "P3\n"
			  << resolutionX << " " << resolutionY << "\n255\n";

	vec3 lookFrom(13, 2, 5);
	vec3 lookAt(0, 0, 0);
	Camera camera(lookFrom, lookAt, vec3(0, 1, 0),20, float(resolutionX) / float(resolutionY), 0.1, (lookFrom - lookAt).length());


	int n = 500;
	hitable** list = new hitable*[n + 1];
	list[0] = new Sphere(vec3(0,-1000, 0), 1000, new Lambertian(vec3(0.5f, 0.5f, 0.5f)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMat = drand48();
			vec3 center(a + 0.9f * drand48(), 0.2, b + 0.9 * drand48());
			if ((center - vec3(4, 0.2f, 0)).length() > 0.9f)
			{
				if (chooseMat < -0.8f)
				{
					list[i++] = new Sphere(center, 0.2, new Lambertian(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
				}
				else if (chooseMat < 0.95)
				{
					list[i++] = new Sphere(center, 0.2, new Metal(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
				}
				else
				{
					list[i++] = new Sphere(center, 0.2f, new Dielectric(1.5f));
				}
			}
		}
	}

	list[i++] = new Sphere(vec3(-12, 1, 0), 1.0f, new Metal(vec3(0.5, 0.8, 0.3) * 0.8f, 0.3f));
	list[i++] = new Sphere(vec3(-8, 1, 0), 1.0f, new Metal(vec3(1, 1, 0.8) * 0.6f, 0.0f));
	list[i++] = new Sphere(vec3(-4, 1, 0), 1.0f, new Metal(vec3(0, 0, 0.6), 0.0f));
	list[i++] = new Sphere(vec3(0, 1, 0), 1.0f, new Metal(vec3(1, 1, 1) , 0.0f));
	list[i++] = new Sphere(vec3(4, 1, 0), 1.0f, new Dielectric(1.5f)); list[i++] = new Sphere(vec3(4, 1, 0), -0.9f, new Dielectric(1.5f));
	list[i++] = new Sphere(vec3(8, 1, 0), 1.0f, new Metal(vec3(0, 1, 0.9) * 0.8f, 0.0f));

	hitable *world = new HitableList(list, i);

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