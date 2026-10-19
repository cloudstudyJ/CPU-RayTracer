#include "math/math.hpp"

#include <cmath>

namespace Math {
    f32 abs(const f32 val) noexcept {
        return std::fabs(val);
    }
    f32 pow(const f32 x, const f32 y) noexcept {
        return std::pow(x, y);
    }
    f32 sqrt(const f32 val) noexcept {
        return std::sqrt(val);
    }
    f32 sin(const f32 rad) noexcept {
        return std::sin(rad);
    }
    f32 cos(const f32 rad) noexcept {
        return std::cos(rad);
    }
    f32 tan(const f32 rad) noexcept {
        return std::tan(rad);
    }
    f32 arccos(const f32 val) noexcept {
        return std::acos(val);
    }
    f32 arctan2(const f32 y, const f32 x) noexcept {
        return std::atan2(y, x);
    }
}