#pragma once

#include <iostream>
#include <cmath>
#include "vec3.h"

void write_color(std::ostream& os, color pixel_color, int samples)
{
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

	double scale = 1.0 / samples;
	r = std::sqrt(r * scale);
	g = std::sqrt(g * scale);
	b = std::sqrt(b * scale);

	os << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '	// max value allowed in ppm is 255.999
	   << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
	   << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}