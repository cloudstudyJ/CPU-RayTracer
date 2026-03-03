#pragma once

#include "vec3.hpp"

class Math {
    public:
        static float toRad(float degree) noexcept { return (PI / 180) * degree; }
        static float toDegree(float rad) noexcept { return (180 / PI) * rad; }

        static float tan(float degree) noexcept;

        static float length(const Vec3& v);
        static float lengthSquare(const Vec3& v);

        static Vec3 normalize(const Vec3& v);
        static Vec3 lerp(const Vec3& a, const Vec3& b, const float t) noexcept;

    public:
        static constexpr float PI  = 3.141'592f;
};