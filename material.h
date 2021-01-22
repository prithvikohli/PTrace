#pragma once

#include "ray.h"
#include "hittable.h"

class material
{
public:
	virtual bool scatter(const ray& r, const hit_data& data, color& attenuation, ray& next) const = 0;
};

class lambertian : public material
{
private:
	color albedo;
public:
	lambertian(const color& albedo) : albedo(albedo) {}
	virtual bool scatter(const ray& r, const hit_data& data, color& attenuation, ray& next) const override
	{
		vec3 dir = data.normal + random_unit_vector();	// Lambertian cosine distribution of reflected rays
		if (dir.near_zero())
			dir = data.normal;		// prevent zero direction vector due to random unit vector generated being exactly opposite to normal vector

		next = ray(data.position, dir);
		attenuation = albedo;
		return true;
	}
};

class metal : public material
{
private:
	color albedo;
	double fuzz;
public:
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
	virtual bool scatter(const ray& r, const hit_data& data, color& attenuation, ray& next) const override
	{
		vec3 dir = reflect(unit_vector(r.direction()), data.normal) + fuzz * random_in_unit_sphere();	// Lambertian cosine distribution of reflected rays
		next = ray(data.position, dir);
		attenuation = albedo;
		return dot(next.direction(), data.normal) > 0;	// ray absorbed if it goes into surface
	}
};

class dielectric : public material
{
private:
	double ir;

	static double reflectance(double cosine, double refractive_ratio)
	{
		double r0 = (1 - refractive_ratio) / (1 + refractive_ratio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow(1 - cosine, 5);
	}
public:
	dielectric(double refractive_index) : ir(refractive_index) {}
	virtual bool scatter(const ray& r, const hit_data& data, color& attenuation, ray& next) const override
	{
		vec3 uv = unit_vector(r.direction());
		double cos_theta = fmin(dot(-uv, data.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
		
		double refract_ratio = data.front_face ? 1.0 / ir : ir;
		bool TIR = refract_ratio * sin_theta > 1.0;		// total internal reflection condition

		vec3 dir;
		if (TIR || reflectance(cos_theta, refract_ratio) > random_double())
			dir = reflect(r.direction(), data.normal);
		else
			dir = refract(unit_vector(r.direction()), data.normal, refract_ratio);

		next = ray(data.position, dir);
		attenuation = color(1.0, 1.0, 1.0);
		return true;
	}
};