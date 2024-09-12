#ifndef __HITABLE__
#define __HITABLE__

#include "ray.h"

class Material;

struct hitRecord
{
	float t;
	vec3 p;
	vec3 normal;
	Material* pMaterial;
};




class hitable
{
public:
	virtual bool hit(const ray& r, float t_min, float t_max, hitRecord& rec) const = 0;
};


class HitableList : public hitable
{
public:
	HitableList() = default;
	HitableList(hitable** l, int n)
	{
		list = l;
		listSize = n;
	}

	virtual bool hit(const ray& r, float t_min, float t_max, hitRecord& rec) const;

	hitable** list;
	int listSize;
};

bool HitableList::hit(const ray& r, float t_min, float t_max, hitRecord& rec) const
{
	hitRecord tmpRec;
	bool hitAnything = false;

	double closestSoFar = t_max;
	for (int i = 0; i < listSize; i++)
	{
		if (list[i]->hit(r, t_min, closestSoFar, tmpRec))
		{
			hitAnything = true;
			closestSoFar = tmpRec.t;
			rec = tmpRec;
		}
	}

	return hitAnything;
}


#endif __HITABLE__