#include "math.hpp"

#include <cmath>

namespace Math {
    float32 abs(const float32 val) noexcept { return std::fabs(val); }

    float32 pow(const float32 x, const float32 y) noexcept { return std::pow(x, y); }
    float32 sqrt(const float32 val) noexcept { return std::sqrt(val); }

    float32 sin(const float32 rad) noexcept { return std::sin(rad); }
    float32 cos(const float32 rad) noexcept { return std::cos(rad); }
    float32 tan(const float32 rad) noexcept { return std::tan(rad); }

    float32 arccos(const float32 val) noexcept { return std::acos(val);}
    float32 arctan2(const float32 y, const float32 x) noexcept { return std::atan2(y, x);}
}