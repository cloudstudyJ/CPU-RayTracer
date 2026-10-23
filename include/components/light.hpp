#pragma once

#include "math/vec3.hpp"
#include "sampling/surface.hpp"

// fwd
class Object;
class Sphere;
class Rectangle;

struct Light {
    using SampleFn = SurfaceSample (*)(const Object&);
    using PDFFn    = f32 (*)(const Object&);

    Vec3     color;
    f32      intensity{};
    SampleFn sample   {};
    PDFFn    pdf      {};
};

template <typename T>
SurfaceSample lightSurfaceSampler(const Object& obj) {
    return sampling::surface(static_cast<const T&>(obj));
}

f32 surfacePDF(const Rectangle& rectangle);
f32 surfacePDF(const Sphere& sphere);

template <typename T>
f32 lightSurfacePDF(const Object& obj) {
    return surfacePDF(static_cast<const T&>(obj));
}
