#include "objects/rectangle.hpp"
#include "solver.hpp"
#include "math/mat4.hpp"

Rectangle::Rectangle() noexcept: Object() {}
Rectangle::Rectangle(const Transform& transform, std::shared_ptr<const Material> material) noexcept
    : Object(transform, material) {
    const Vec4& r = transform.rotation;

    const Vec3 axis = { .data = { r.x, r.y, r.z } };
    const Mat4 Mw   = Mat4::translate(transform.position) * Mat4::rotate(axis, Math::toRad(r.w)) * Mat4::scale(transform.scale);

    Vec3 u = Mw * Vec3{ .data = { 1.0f, 0.0f, 0.0f } };
    Vec3 v = Mw * Vec3{ .data = { 0.0f, 1.0f, 0.0f } };

    build(transform.position, u, v);
}
Rectangle::Rectangle(const Vec3& pos, const Vec3& u, const Vec3& v, std::shared_ptr<const Material> material) noexcept
    : Object({ .position = pos, .scale = { .data = { 1.0f, 1.0f, 1.0f } } }, material) { build(pos, u, v); }

HitResult Rectangle::hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const {
    return Solver::hitTest(ray, *this, tMin, tMax);
}

// p3 ------ p2
// |         |
// |    c    |
// |         |
// p0 ------ p1
void Rectangle::build(const Vec3& pos, const Vec3& u, const Vec3& v) noexcept {
    const Vec3 p[4] = {
        pos - u * 0.5f - v * 0.5f,
        pos + u * 0.5f - v * 0.5f,
        pos + u * 0.5f + v * 0.5f,
        pos - u * 0.5f + v * 0.5f
    };

    Vec3 min = p[0];
    Vec3 max = p[0];

    for (const auto& v: p) {
        min.x = Utility::min(min.x, v.x);
        min.y = Utility::min(min.y, v.y);
        min.z = Utility::min(min.z, v.z);

        max.x = Utility::max(max.x, v.x);
        max.y = Utility::max(max.y, v.y);
        max.z = Utility::max(max.z, v.z);
    }

    mU    = u;
    mV    = v;
    mAABB = { min, max };
}