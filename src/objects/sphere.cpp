#include "objects/sphere.hpp"
#include "solver.hpp"

Sphere::Sphere() noexcept
    : Object(), mRadius(1.0f) {
    mAABB = {
        mTransform.position - mRadius,
        mTransform.position + mRadius
    };
}
Sphere::Sphere(const Transform& transform, const f32 r, std::shared_ptr<const Material> material) noexcept
    : Object(transform, material), mRadius(r) {
    mAABB = {
        mTransform.position - mRadius,
        mTransform.position + mRadius
    };
}

HitResult Sphere::hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const { return Solver::hitTest(ray, *this, tMin, tMax); }