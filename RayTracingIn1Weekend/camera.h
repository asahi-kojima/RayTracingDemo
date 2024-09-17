#ifndef __CAMERA__
#define __CAMERA__

#include "util.h"
#include "ray.h"


vec3 randominUnitDisk()
{
	vec3 p;
	do
	{
		p = 2.0 * vec3(randomF64(), randomF64(), 0) - vec3(1, 1, 0);
	} while(dot(p, p) >= 1.0f);

	return p;
}


class Camera
{
public:
	Camera()
	{
		origin			= vec3(0.0, 0.0, 0.0);
		lowerLeftCorner	= vec3(-2.0f, -1.0f, -1.0f);
		horizontal		= vec3(4.0f, 0.0f, 0.0f);
		vertical		= vec3(0.0f, 2.0f, 0.0f);
	}

	Camera(float vfov, float aspect)
	{
		float theta 		= vfov * M_PI / 180.0f;
		float halfHeight 	= tan(theta / 2);
		float halfWidth 	= aspect * halfHeight;

		origin 			= vec3(0, 0, 0);
		lowerLeftCorner = vec3(-halfWidth, -halfHeight, -1.0f);
		horizontal 		= vec3(2 * halfWidth, 0, 0);
		vertical		= vec3(0, 2 * halfHeight, 0);
	}

	Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vfov, float aspect, float zoom = 1, float aperture = 0, float focusDist = 1)
	{
		lensRadius = aperture / 2;

		float theta 		= vfov * M_PI / 180.0f;
		float halfHeight 	= tan(theta / 2);
		float halfWidth 	= aspect * halfHeight;

		origin = lookFrom;
		w = normalize(lookFrom - lookAt);//z
		u = normalize(cross(vUp, w));//x
		v = cross(w, u);//y

		lowerLeftCorner = origin - zoom * w - halfWidth * u - halfHeight * v;
		horizontal 		= 2 * halfWidth * u;
		vertical		= 2 * halfHeight * v;
	}


	ray getRay(float s, float t)
	{
		vec3 rd = lensRadius * randominUnitDisk();
		vec3 offset = u * rd.x() + v * rd.y();
		vec3 rayOrigin = origin + offset;
		return ray(rayOrigin, lowerLeftCorner + s * horizontal + t * vertical - rayOrigin);
	}


	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
 	float lensRadius;
};


#endif __CAMERA__