#include "objects/quad.hpp"
#include "solver.hpp"

Quad::Quad() noexcept: Object() {}
Quad::Quad(const Vec3& pos, const Vec3& a, const Vec3& b, const Material* material) noexcept
    : Object(pos, material), mU(a), mV(b) {
    mAABB = AABB::unite(
        {pos, pos + a + b},
        {pos + a, pos + b}
    );
}

HitResult Quad::hitTest(const Ray& ray, const float32 tMin, const float32 tMax) const { return Solver::hitTest(ray, *this, tMin, tMax); }