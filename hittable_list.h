#pragma once

#include <memory>
#include <vector>
#include "hittable.h"

class hittable_list : public hittable
{
private:
	std::vector<std::shared_ptr<hittable>> objects;
public:
	hittable_list() {}
	hittable_list(std::shared_ptr<hittable> obj) { add(obj); }

	void add(std::shared_ptr<hittable> obj) { objects.push_back(obj); }
	void clear() { objects.clear(); }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_data& data) const override;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_data& data) const
{
	hit_data temp_data;
	bool hit_anything = false;
	double closest = t_max;

	for (const std::shared_ptr<hittable>& obj : objects)
	{
		if (obj->hit(r, t_min, closest, temp_data))
		{
			hit_anything = true;
			closest = temp_data.t;
			data = temp_data;
		}
	}

	return hit_anything;
}