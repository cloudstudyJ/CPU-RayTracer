#pragma once

#include "vec3.hpp"

class Texture {
    public:
        virtual Vec3 color(const float32 u, const float32 v) const noexcept = 0;
};