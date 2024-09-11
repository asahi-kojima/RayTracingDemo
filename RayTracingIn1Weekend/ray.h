#ifndef __RAY__
#define __RAY__

#include "vec3.h"

class ray
{
public:
	ray() = default;
	ray(const  vec3& a, const vec3& b){mA = a; mB = b;}

	vec3 origin() const {return mA;};
	vec3 direction() const {return mB;}
	vec3 pointAt(float t) const {return mA + mB * t;}

	vec3 mA;
	vec3 mB;
};


#endif __RAY__