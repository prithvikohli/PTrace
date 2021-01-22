#pragma once

#include <iostream>
#include <cmath>

class vec3
{
private:
	double e[3];
public:
	vec3() : e{ 0, 0, 0 } {}
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() { return vec3(-e[0], -e[1], -e[2]); }

	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	vec3& operator+=(const vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];

		return *this;
	}

	vec3& operator-=(const vec3& v)
	{
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];

		return *this;
	}

	vec3& operator*=(const double f)
	{
		e[0] *= f;
		e[1] *= f;
		e[2] *= f;

		return *this;
	}

	vec3& operator/=(const double f) { return (*this *= 1 / f); }

	double length_squared() const
	{
		return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
	}

	double length() const { return std::sqrt(length_squared()); }

	bool near_zero() const	// check if vector is very close to 0 in all dimensions
	{
		double lim = 1.0e-8;
		return fabs(e[0]) < lim && fabs(e[1]) < lim && fabs(e[2]) < lim;
	}
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream& os, vec3& v)
{
	return os << v.x() << ' ' << v.y() << ' ' << v.z() << ' ';
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline vec3 operator*(const vec3& v, double f)
{
	return vec3(v.x() * f, v.y() * f, v.z() * f);
}

inline vec3 operator*(double f, const vec3& v)
{
	return v * f;
}

inline vec3 operator/(const vec3& v, double f)
{
	return v * (1 / f);
}

inline double dot(const vec3& u, const vec3& v)
{
	return u.x() * v.x()
		 + u.y() * v.y()
		 + u.z() * v.z();
}

inline vec3 cross(const vec3& u, const vec3& v)
{
	return vec3(
		u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(),
		u.x() * v.y() - u.y() * v.x()
	);
}

inline vec3 unit_vector(const vec3& v)
{
	return v / v.length();
}

inline vec3 random_vec3(double min, double max)
{
	vec3 v(random_double(min, max), random_double(min, max), random_double(min, max));
	return v;
}

inline vec3 random_in_unit_sphere()
{
	while (true)
	{
		vec3 v = random_vec3(-1.0, 1.0);
		if (v.length_squared() < 1.0)
			return v;
	}
}

inline vec3 random_unit_vector()
{
	return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_unit_disk()
{
	while (true)
	{
		vec3 v(random_double(-1.0, 1.0), random_double(-1.0, 1.0), 0.0);
		if (v.length_squared() < 1)
			return v;
	}
}

inline vec3 reflect(const vec3& v, const vec3& normal)
{
	return v - 2 * dot(v, normal) * normal;
}

vec3 refract(vec3 uv, const vec3& normal, double ratio)
{
	vec3 minus_uv = -uv;
	double cos_theta = fmin(dot(minus_uv, normal), 1.0);	// fmin with 1.0 to prevent value > 1.0 due to floating point imprecision
	vec3 r_out_perp = ratio * (uv + cos_theta * normal);
	vec3 r_out_parallel = -std::sqrt(fabs(1.0 - r_out_perp.length_squared())) * normal;
	return r_out_perp + r_out_parallel;
}