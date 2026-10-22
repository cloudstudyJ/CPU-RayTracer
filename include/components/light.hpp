#pragma once

#include "math/vec3.hpp"
#include "sampling/surface.hpp"

// fwd
class Object;

struct Light {
    using SampleFn = SurfaceSample (*)(const Object&);

    Vec3     color;
    f32      intensity{};
    SampleFn sample   {};
};

template <typename T>
SurfaceSample lightSurfaceSampler(const Object& obj) noexcept {
    return sampling::surface(static_cast<const T&>(obj));
}