#ifndef __CAMERA__
#define __CAMERA__




#include "ray.h"

class Camera
{
public:
	Camera()
	{
		origin=vec3(0.0, 0.0, 0.0);
		lowerLeftCorner=vec3(-2.0f, -1.0f, -1.0f);
		horizontal=vec3(4.0f, 0.0f, 0.0f);
		vertical=vec3(0.0f, 2.0f, 0.0f);
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
		return ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
	}


	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
};


#endif __CAMERA__