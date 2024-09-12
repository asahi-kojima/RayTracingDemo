#ifndef __VEC3__
#define __VEC3__

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "util.h"

class vec3
{
public:
	vec3()=default;

	vec3(float e0, float e1, float e2) 
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}

	float x() const {return e[0];}
	float y() const {return e[1];}
	float z() const {return e[2];}
	float r() const {return e[0];}
	float g() const {return e[1];}
	float b() const {return e[2];}


	const vec3& operator+() const{return *this;}
	vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
	float operator[](int i) const {return e[i];}
	float& operator[](int i) {return e[i];}

	vec3& operator+=(const vec3& v2);
	vec3& operator-=(const vec3& v2);
	vec3& operator*=(const vec3& v2);
	vec3& operator/=(const vec3& v2);
	vec3& operator*=(const float t);
	vec3& operator/=(const float t);

	float length() const {return sqrt(e[0]*e[0] + e[1]*e[1]+e[2]*e[2]);}
	float squaredLength() const {return e[0]*e[0] + e[1]*e[1]+e[2]*e[2];}
	void normalize();

	float e[3];
};




inline vec3& vec3::operator+=(const vec3& v)
{
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline vec3& vec3::operator-=(const vec3& v)
{
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const vec3& v)
{
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline vec3& vec3::operator/=(const vec3& v)
{
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline vec3& vec3::operator*=(const float t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/=(const float t)
{
	float k = 1 / t;
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline void vec3::normalize()
{
	float k = 1 / length();
	this->operator*=(k);
}

inline vec3 normalize(vec3 v)
{
	v.normalize();
	return v;
}


inline vec3 operator+(const vec3& a, const vec3& b)
{
	vec3 newVec3(a);
	newVec3 += b;
	return newVec3;
}

inline vec3 operator-(const vec3& a, const vec3& b)
{
	vec3 newVec3(a);
	newVec3 -= b;
	return newVec3;
}

inline vec3 operator*(const vec3& a, const vec3& b)
{
	vec3 newVec3(a);
	newVec3 *= b;
	return newVec3;
}

inline vec3 operator*(const vec3& a, float t)
{
	vec3 newVec3(a);
	newVec3 *= t;
	return newVec3;
}


inline vec3 operator*(float t, const vec3& a)
{
	vec3 newVec3(a);
	newVec3 *= t;
	return newVec3;
}

inline vec3 operator/(const vec3& a, float t)
{
	vec3 newVec3(a);
	newVec3 /= t;
	return newVec3;
}

inline float dot(const vec3& a, const vec3& b)
{
	float result = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		result += a.e[i] * b.e[i];
	}
	return result;
}




inline vec3 randomInUnitSphere()
{
	vec3 p;
	do
	{
		p = 2.0f * vec3(randomF64(), randomF64(), randomF64()) - vec3(1,1,1);
	} while (p.squaredLength() >= 1.0f);
	return p;
}


inline vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}


#endif __VEC3__