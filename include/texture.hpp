#pragma once

#include "math/vec3.hpp"

class Texture {
    public:
        virtual Vec3 color(const f32 u, const f32 v) const noexcept = 0;
};