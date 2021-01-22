#pragma once

#include "common.h"
#include "ray.h"

class camera
{
private:
	point3 origin;
	point3 lower_left_corner;	// lower left corner of image plane
	vec3 horizontal;
	vec3 vertical;
	vec3 w, u, v;
	double lens_radius;
public:
	camera(double vfov, double aspect_ratio, double aperture, double focus_dist, point3 lookfrom, point3 lookat, vec3 vup)
	{
		double theta = deg_to_rad(vfov);
		double h = tan(theta / 2) * focus_dist;
		double viewport_height = 2.0 * h;
		double viewport_width = aspect_ratio * viewport_height;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = viewport_width * u;
		vertical = viewport_height * v;
		lower_left_corner = origin - (horizontal / 2) - (vertical / 2) - w * focus_dist;

		lens_radius = aperture / 2;
	}

	ray get_ray(double s, double t) const
	{
		point3 p = random_in_unit_disk() * lens_radius;
		vec3 offset = u * p.x() + v * p.y();

		return ray(origin + offset, (lower_left_corner + s * horizontal + t * vertical) - origin - offset);
	}
};