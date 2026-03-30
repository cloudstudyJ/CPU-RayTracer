#include "objects/box.hpp"
#include "utility.hpp"

Box::Box() noexcept
    : Object() {}
Box::Box(const Vec3& a, const Vec3& b, std::shared_ptr<const Material> material) noexcept
    : Object((a + b) / 2, material) {
    Vec3 min = {Utility::min(a.x, b.x), Utility::min(a.y, b.y), Utility::min(a.z, b.z)};
    Vec3 max = {Utility::max(a.x, b.x), Utility::max(a.y, b.y), Utility::max(a.z, b.z)};

    Vec3 dx = {max.x - min.x, 0.0f, 0.0f};
    Vec3 dy = {0.0f, max.y - min.y, 0.0f};
    Vec3 dz = {0.0f, 0.0f, max.z - min.z};

    mFaces[0] = Quad(Vec3{min.x, min.y, max.z},  dx,  dy, material);
    mFaces[1] = Quad(Vec3{max.x, min.y, max.z}, -dz,  dy, material);
    mFaces[2] = Quad(Vec3{max.x, min.y, min.z}, -dx,  dy, material);
    mFaces[3] = Quad(Vec3{min.x, min.y, min.z},  dz,  dy, material);
    mFaces[4] = Quad(Vec3{min.x, max.y, max.z},  dx, -dz, material);
    mFaces[5] = Quad(Vec3{min.x, min.y, min.z},  dx,  dz, material);

    mAABB = {min, max};
}

HitResult Box::hitTest(const Ray& ray, const f32 tMin, const f32 tMax) const {
    HitResult hitResult;
    hitResult.hitT = tMax;

    for (const auto& quad: mFaces) {
        HitResult quadHitResult = quad.hitTest(ray, tMin, hitResult.hitT);

        if (quadHitResult.isHit && (quadHitResult.hitT < hitResult.hitT))
            hitResult = quadHitResult;
    }

    return hitResult;
}