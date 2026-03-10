#include "aabb.hpp"
#include "utility.hpp"
#include "constants.hpp"

AABB::AABB() noexcept
    : min(INF_F32, INF_F32, INF_F32), max(-INF_F32, -INF_F32, -INF_F32) {}
AABB::AABB(const Vec3& min, const Vec3& max) noexcept
    : min(min), max(max) {}

AABB AABB::unite(const AABB& a, const AABB& b) noexcept {
    return {
        {
            Utility::min(a.min.x, b.min.x),
            Utility::min(a.min.y, b.min.y),
            Utility::min(a.min.z, b.min.z)
        },
        {
            Utility::max(a.max.x, b.max.x),
            Utility::max(a.max.y, b.max.y),
            Utility::max(a.max.z, b.max.z)
        }
    };
}