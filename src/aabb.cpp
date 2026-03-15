#include "aabb.hpp"
#include "utility.hpp"
#include "constants.hpp"

AABB::AABB() noexcept
    : min(INF_F32, INF_F32, INF_F32), max(-INF_F32, -INF_F32, -INF_F32) {}
AABB::AABB(const Vec3& a, const Vec3& b) noexcept {
    min = {
        Utility::min(a.x, b.x),
        Utility::min(a.y, b.y),
        Utility::min(a.z, b.z)
    };
    max = {
        Utility::max(a.x, b.x),
        Utility::max(a.y, b.y),
        Utility::max(a.z, b.z)
    };
}

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