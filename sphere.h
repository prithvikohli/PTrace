#pragma once

#include "hittable.h"

class sphere : public hittable
{
private:
	point3 center;
	double radius = 0.0;
	std::shared_ptr<material> mat;
public:
	sphere() {}
	sphere(point3 cent, double rad, std::shared_ptr<material> mat) : center(cent), radius(rad), mat(mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_data& data) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_data& data) const
{
	// parameters of sphere intersection equation
	vec3 oc = r.origin() - center;
	double a = r.direction().length_squared();
	double half_b = dot(r.direction(), oc);
	double c = dot(oc, oc) - radius * radius;

	double discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
		return false;
	double sqrtd = std::sqrt(discriminant);
	
	double root = (-half_b - sqrtd) / a;
	if (root < t_min || root > t_max)
	{
		root = (-half_b + sqrtd) / a;
		if (root < t_min || root > t_max)
			return false;
	}

	data.t = root;
	data.position = r.at(root);

	vec3 outward_normal = (data.position - center) / radius;
	data.set_face_normal(r, outward_normal);

	data.mat = mat;

	return true;
}