#pragma once

#include "math/vec2.hpp"
#include "math/vec3.hpp"

class Texture {
    Texture(const Texture&)                = delete;
    Texture(Texture&&) noexcept            = delete;
    Texture& operator=(const Texture&)     = delete;
    Texture& operator=(Texture&&) noexcept = delete;

    public:
        virtual ~Texture() noexcept = default;

        virtual Vec3 sample(const Vec2& uv) const noexcept = 0;

    protected:
        Texture() noexcept = default;
};