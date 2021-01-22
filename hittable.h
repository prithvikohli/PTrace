#pragma once

#include <memory>
#include "ray.h"

class material;

struct hit_data {
	point3 position;
	vec3 normal;
	std::shared_ptr<material> mat;
	double t;
	bool front_face;	// is this hit on a front face from our perspective?

	inline void set_face_normal(const ray& r, vec3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;	// if not a front face, set normal to point inwards
	}
};

class hittable
{
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_data& data) const = 0;
};