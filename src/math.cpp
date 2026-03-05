#include "math.hpp"

#include <cmath>

float32 Math::sqrt(const float32 val) noexcept { return std::sqrt(val); }
float32 Math::tan(const float32 degree) noexcept { return std::tan(toRad(degree)); }