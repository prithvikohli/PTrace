#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

// constants
// ---------
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

// utility functions
// -----------------
inline double deg_to_rad(double angle)
{
	return angle * (pi / 180);
}

inline double random_double()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double min, double max)
{
	static std::uniform_real_distribution<double> distribution(min, max);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double clamp(double val, double min, double max)
{
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;
}