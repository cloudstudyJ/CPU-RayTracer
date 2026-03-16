#pragma once

#include "vec3.hpp"

// fwd
class BxDF;
class Texture;

struct Material {
    const BxDF*    bxdf{};
    const Texture* color{};

    Vec3 emitColor;
    float32 emitIntensity{};
};