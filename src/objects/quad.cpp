#include "objects/quad.hpp"
#include "solver.hpp"

Quad::Quad() noexcept: Object() {}
Quad::Quad(const Transform& transform, const Vec3& a, const Vec3& b, std::shared_ptr<const Material> material) noexcept
    : Object(transform, material), mU(a), mV(b) {
    mAABB = AABB::unite(
        {transform.position, transform.position + a + b},
        {transform.position + a, transform.position + b}
    );
}

HitResult Quad::hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const { return Solver::hitTest(ray, *this, tMin, tMax); }