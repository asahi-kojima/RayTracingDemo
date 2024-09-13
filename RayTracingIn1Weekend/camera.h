#ifndef __CAMERA__
#define __CAMERA__


#include "util.h"
#include "ray.h"

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

	Camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vfov, float aspect, float zoom = 1)
	{
		vec3 u, v, w;

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

	Camera(vec3 origin, vec3 lowerLeftCorner, vec3 horizontal, vec3 vertical)
	:origin(origin)
	,lowerLeftCorner(lowerLeftCorner)
	,horizontal(horizontal)
	,vertical(vertical)
	{

	}

	ray getRay(float u, float v)
	{
		//if (isInRange(u, 0.0f, 1.0f) && isInRange(v, 0.0f, 1.0f))
		{
			return ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
		}
		//else
		{
			std::cout << "range error\n";
			exit(1);
		}
	}


	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
};


#endif __CAMERA__