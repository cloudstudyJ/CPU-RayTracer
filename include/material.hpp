#pragma once

#include "vec3.hpp"

// fwd
class BxDF;
class Texture;

struct Material {
    const BxDF*    bxdf{};
    const Texture* texture{};

    Vec3 emitColor;
    f32 emitIntensity{};
};