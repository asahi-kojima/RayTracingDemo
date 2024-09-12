#ifndef __MATERIAL__
#define __MATERIAL__

#include "hitable.h"
#include "util.h"

class Material
{
public:
	virtual bool scatter(const ray& rayIn, const hitRecord& rec, vec3& attenuation, ray& scattered) const = 0;
};



class Lambertian : public Material
{
public:
	Lambertian(const vec3& a) : albedo(a){}

	virtual bool scatter(const ray& rayIn, const hitRecord& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 target = rec.p + rec.normal + randomInUnitSphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	vec3 albedo;
};

class Metal : public Material
{
public:
	Metal(const vec3& a, float f = 0) : albedo(a) , fuzz(f < 1 ? f : 1){}

	virtual bool scatter(const ray& rayIn, const hitRecord& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 reflected = reflect(normalize(rayIn.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * randomInUnitSphere());
		attenuation = albedo;
		return  (dot(scattered.direction(), rec.normal) > 0);
	}

	vec3 albedo;
	float fuzz;
};




#endif __MATERIAL__