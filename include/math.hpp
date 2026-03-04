#pragma once

#include "types.hpp"
#include "vec3.hpp"

class Math {
    public:
        static float32 toRad(const float32 degree) noexcept { return (PI / 180) * degree; }
        static float32 toDegree(const float32 rad) noexcept { return (180 / PI) * rad; }

        static float32 sqrt(const float32 val);
        static float32 tan(const float32 degree) noexcept;

        static float32 length(const Vec3& v);
        static float32 lengthSquare(const Vec3& v);

        static Vec3 normalize(const Vec3& v);
        static Vec3 lerp(const Vec3& a, const Vec3& b, const float32 t) noexcept;

    public:
        static constexpr float32 PI = 3.141'592f;
};