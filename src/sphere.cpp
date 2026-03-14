#include "objects/sphere.hpp"
#include "solver.hpp"

Sphere::Sphere() noexcept
    : Object(), mRadius(1.0f) {
    mAABB = {
        mPos - mRadius,
        mPos + mRadius
    };
}
Sphere::Sphere(const Vec3& pos, const float32 r, std::shared_ptr<Material> material) noexcept
    : Object(pos, material), mRadius(r) {
    mAABB = {
        mPos - mRadius,
        mPos + mRadius
    };
}

HitResult Sphere::hitTest(const Ray& ray, const float32 tMin, const float32 tMax) const { return Solver::hitTest(ray, *this, tMin, tMax); }