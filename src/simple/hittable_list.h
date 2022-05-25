#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
// https://www.runoob.com/w3cnote/cpp-vector-container-analysis.html 任意类型的动态数组
#include <vector>

// https://blog.csdn.net/u013360881/article/details/71798950 共享内存
using std::make_shared;
using std::shared_ptr;

class hittable_list
{
public:
    std::vector<shared_ptr<hittable>> objects;

public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object) }
    void clear() { objects.clear() }
    void add(shared_ptr<hittable> object) { objects.push_back(object) }
    virtual bool hit(
        const ray &r, double t_min, double t_max, hit_record &rec) const override;
};

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    auto closet_so_far = t_max;

    for (const auto &object : objects)
    {
        if (object->hit(r, t_min, closet_so_far, temp_rec))
        {
            hit_anything = true;
            closet_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

#endif