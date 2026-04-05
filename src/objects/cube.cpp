#include "objects/cube.hpp"
#include "math/mat4.hpp"

Cube::Cube() noexcept
    : Object() {}
Cube::Cube(const Transform& transform, std::shared_ptr<const Material> material) noexcept
    : Object(transform, material) {
    const Vec4& r = transform.rotation;

    const Vec3 axis = { .data = { r.x, r.y, r.z } };
    const Mat4 Mw   = Mat4::translate(transform.position) * Mat4::rotate(axis, Math::toRad(r.w)) * Mat4::scale(transform.scale);

    Vec3 c = transform.position;
    Vec3 u = Mw * Vec3{ .data = { 1.0f, 0.0f, 0.0f } };
    Vec3 v = Mw * Vec3{ .data = { 0.0f, 1.0f, 0.0f } };
    Vec3 w = Mw * Vec3{ .data = { 0.0f, 0.0f, 1.0f } };

    mFaces[0] = Rectangle(c + w * 0.5f, u, v, material);    // front
    mFaces[1] = Rectangle(c + u * 0.5f, v, w, material);    // right
    mFaces[2] = Rectangle(c + v * 0.5f, w, u, material);    // top
    mFaces[3] = Rectangle(c - u * 0.5f, w, v, material);    // left
    mFaces[4] = Rectangle(c - v * 0.5f, u, w, material);    // bottom
    mFaces[5] = Rectangle(c - w * 0.5f, v, u, material);    // back

    mAABB = mFaces[0].aabb();
    for (i32 i = 1; i < 6; ++i)
        mAABB = AABB::unite(mAABB, mFaces[i].aabb());
}

HitResult Cube::hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const {
    HitResult hitResult;
    hitResult.hitT = tMax;

    for (const auto& quad: mFaces) {
        HitResult quadHitResult = quad.hitTest(ray, tMin, hitResult.hitT);

        if (quadHitResult.isHit && (quadHitResult.hitT < hitResult.hitT))
            hitResult = quadHitResult;
    }

    return hitResult;
}