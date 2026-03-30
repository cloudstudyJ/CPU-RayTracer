#pragma once

#include "accelerationStructure.hpp"
#include "objects/object.hpp"
#include "components/light.hpp"

#include <utility>
#include <memory>

// fwd
struct Ray;

class World {
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    public:
        World() noexcept = default;
        World(World&& other) noexcept;
        ~World() noexcept;

        World& operator=(World&& other) noexcept;

        void buildAccelerationStructure() { mAS.build(mObjects); }

        template <typename T, typename ... Args>
        T* addObject(Args&& ... args);

        void addLight(Object* obj, const Light& lightInfo) { mLights.push_back(std::pair<Object*, Light>(obj, lightInfo)); }
        void clearObjects() noexcept;

        HitResult traceRay(const Ray& ray, const f32 tMin, const f32 tMax) noexcept { return mAS.hitTest(ray, tMin, tMax); }

        const std::vector<std::pair<Object*, Light>>& getLights() const noexcept { return mLights; }

    private:
        AccelerationStructure mAS;

        std::vector<std::unique_ptr<Object>>   mObjects;
        std::vector<std::pair<Object*, Light>> mLights;
};

template <typename T, typename ... Args>
T* World::addObject(Args&& ... args) {
    static_assert(std::is_base_of_v<Object, T>, "Template type T must be derived from Object!");

    auto newObject = std::make_unique<T>(std::forward<Args>(args)...);
    mObjects.emplace_back(std::move(newObject));

    return static_cast<T*>(mObjects.back().get());
}