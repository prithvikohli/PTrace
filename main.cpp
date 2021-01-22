#include <iostream>
#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

color ray_color(const ray& r, const hittable& scene, int depth)
{
	if (depth <= 0)	// reached maximum number of bounces
		return color(0.0, 0.0, 0.0);

	hit_data data;
	if (scene.hit(r, 0.001, infinity, data))	// 0.001 to compensate for floating-point imprecision and prevent shadow acne
	{
		ray next;
		color attenuation;
		if (data.mat->scatter(r, data, attenuation, next))
			return attenuation * ray_color(next, scene, depth - 1);
		return color(0.0, 0.0, 0.0);	// if a ray is not scattered, it is absorbed
	}
		
	vec3 unit_direction = unit_vector(r.direction());
	double t = 0.5 * (unit_direction.y() + 1.0);
	return (1 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);	// linearly interpolate to create white-to-blue gradient for ambient light (sky)
}

int main()
{
	// setup image
	// -----------
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width  = 1920;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;	// maximum number of bounces for a ray

	// setup scene
	// -----------
	point3 lookfrom = point3(0, 0, 0);
	point3 lookat = point3(0, 0, -1);
	double focus_dist = (lookat - lookfrom).length();
	camera cam(90.0, 16.0 / 9.0, 0.0, focus_dist, lookfrom, lookat, vec3(0, 1, 0));

	std::shared_ptr<lambertian> ground = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
	std::shared_ptr<lambertian> center = std::make_shared<lambertian>(color(0.7, 0.3, 0.3));
	std::shared_ptr<dielectric> left = std::make_shared<dielectric>(1.5);
	std::shared_ptr<metal> right = std::make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

	hittable_list scene;
	scene.add(std::make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, ground));
	scene.add(std::make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, center));
	scene.add(std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, left));
	scene.add(std::make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, right));

	// render
	// ------
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j)
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i)
		{
			color pixel_color;
			for (int s = 0; s < samples_per_pixel; ++s)
			{
				double u = (i + random_double()) / (image_width  - 1);
				double v = (j + random_double()) / (image_height - 1);

				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, scene, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone!\n";

	return 0;
}