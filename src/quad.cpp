#include "objects/quad.hpp"
#include "solver.hpp"

Quad::Quad() noexcept: Object() {}
Quad::Quad(const Vec3& pos, const Vec3& u, const Vec3& v, std::shared_ptr<Material> material) noexcept
    : Object(pos, material), mU(u), mV(v) {
    mAABB = AABB::unite(
        {pos, pos + u + v},
        {pos + u, pos + v}
    );
}

HitResult Quad::hitTest(const Ray& ray, const float32 tMin, const float32 tMax) const { return Solver::hitTest(ray, *this, tMin, tMax); }