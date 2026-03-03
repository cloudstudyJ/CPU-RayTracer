#include "math.hpp"

#include <cmath>

float32 Math::tan(const float32 degree) noexcept { return std::tan(toRad(degree)); }

float32 Math::length(const Vec3& v) { return std::sqrtf(lengthSquare(v)); }
float32 Math::lengthSquare(const Vec3& v) { return ((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }

Vec3 Math::normalize(const Vec3& v) { return (v / length(v)); }
Vec3 Math::lerp(const Vec3& a, const Vec3& b, const float32 t) noexcept { return (a + (b - a) * t); }