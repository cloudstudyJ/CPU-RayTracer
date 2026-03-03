#include "math.hpp"

#include <cmath>

float Math::tan(float degree) noexcept { return std::tan(toRad(degree)); }

float Math::length(const Vec3& v) { return std::sqrtf(lengthSquare(v)); }
float Math::lengthSquare(const Vec3& v) { return ((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }

Vec3 Math::normalize(const Vec3& v) { return (v / length(v)); }
Vec3 Math::lerp(const Vec3& a, const Vec3& b, const float t) noexcept { return (a + (b - a) * t); }