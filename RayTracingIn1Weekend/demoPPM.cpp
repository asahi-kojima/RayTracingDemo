#include <iostream>
#include "ray.h"


bool hitSphere(const vec3& center, float radius, const ray& r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2 * dot(r.direction(), oc);
	float c = dot(oc, oc) -radius * radius;
	float D = b * b - 4 * a * c;
	return (D > 0.0f);
}

vec3 color(const ray& r)
{
	if(hitSphere(vec3(0, 0, -1), 0.5f, r))
	{
		return vec3(1, 0, 0);
	}

	//ヒットしない場合:背景色
	vec3 unitDirection = normalize(r.direction());
	float t = 0.5f * (unitDirection.y() + 1.0f);

	return vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + vec3(0.5f, 0.7f, 1.0f) * t;
}

int main()
{
    int nx = 600;
    int ny = 300;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
	vec3 horizontal(4.0f, 0.0f, 0.0f);
	vec3 vertical(0.0f, 2.0f, 0.0f);
	vec3 origin(0.0, 0.0, 0.0);

    for (int j = ny - 1; j >= 0; j--)//縦
    {
        for (int i = 0; i < nx; i++)//横
        {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);

			ray r(origin, lowerLeftCorner + u*horizontal + v*vertical);
			vec3 col = color(r);


            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}