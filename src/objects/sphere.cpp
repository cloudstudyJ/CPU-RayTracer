#include "objects/sphere.hpp"
#include "solver.hpp"

Sphere::Sphere() noexcept
    : Object(), mRadius(1.0f) {
    mAABB = {
        mPos - mRadius,
        mPos + mRadius
    };
}
Sphere::Sphere(const Vec3& pos, const f32 r, const Material* material) noexcept
    : Object(pos, material), mRadius(r) {
    mAABB = {
        mPos - mRadius,
        mPos + mRadius
    };
}

HitResult Sphere::hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const { return Solver::hitTest(ray, *this, tMin, tMax); }