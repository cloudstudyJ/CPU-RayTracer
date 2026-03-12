#pragma once

#include "accelerationStructure.hpp"
#include "object.hpp"
#include "ray.hpp"

#include <vector>

class World {
    public:
        ~World() noexcept {
            for (Object* object: mObjects)
                delete object;
        }

        void buildAccelerationStructure() { mAccelerationStructure.build(mObjects); }

        template <typename T, typename ... Args>
        T* addObject(Args&& ... args);

        HitResult traceRay(const Ray& ray, const float32 tMin, const float32 tMax) noexcept { return mAccelerationStructure.hitTest(ray, tMin, tMax); }

    private:
        AccelerationStructure mAccelerationStructure;

        std::vector<Object*> mObjects;
};

template <typename T, typename ... Args>
T* World::addObject(Args&& ... args) {
    static_assert(std::is_base_of_v<Object, T>, "Template type T must be derived from Object!");

    T* newObject = new T(std::forward<Args>(args)...);
    mObjects.push_back(newObject);

    return newObject;
}