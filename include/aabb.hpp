#pragma once

#include "math/vec3.hpp"
#include "utility.hpp"
#include "constants.hpp"

struct AABB {
    public:
        constexpr AABB() noexcept;
        constexpr AABB(const Vec3& a, const Vec3& b) noexcept;

        static constexpr AABB unite(const AABB& a, const AABB& b) noexcept;

    public:
        Vec3 min;
        Vec3 max;
};

constexpr AABB::AABB() noexcept
    : min{ .data = {  INF_F32,  INF_F32,  INF_F32 } }
    , max{ .data = { -INF_F32, -INF_F32, -INF_F32 } } {}
constexpr AABB::AABB(const Vec3& a, const Vec3& b) noexcept
    : min{ .data = { Utility::min(a.x, b.x), Utility::min(a.y, b.y), Utility::min(a.z, b.z) } }
    , max{ .data = { Utility::max(a.x, b.x), Utility::max(a.y, b.y), Utility::max(a.z, b.z) } } {}

constexpr AABB AABB::unite(const AABB& a, const AABB& b) noexcept {
    return {
        {
            .data = {
                Utility::min(a.min.x, b.min.x),
                Utility::min(a.min.y, b.min.y),
                Utility::min(a.min.z, b.min.z)
            }
        },
        {
            .data = {
                Utility::max(a.max.x, b.max.x),
                Utility::max(a.max.y, b.max.y),
                Utility::max(a.max.z, b.max.z)
            }
        }
    };
}