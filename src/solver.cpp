#include "solver.hpp"
#include "utility.hpp"
#include "objects/sphere.hpp"
#include "objects/quad.hpp"
#include "aabb.hpp"
#include "math/math.hpp"
#include "ray.hpp"

HitResult Solver::hitTest(const Ray& ray, const Sphere& sphere, const f32 tMin, const f32 tMax) {
    Vec3 spherePos = sphere.transform().position;
    f32 radius = sphere.radius();

    Vec3 ac = spherePos - ray.origin;
    f32 a = ray.direction.lengthSquare();
    f32 h = Vec3::dot(ray.direction, ac);
    f32 c = ac.lengthSquare() - radius * radius;

    f32 discriminant = h * h - a * c;
    if (discriminant < -EPS_F32)
        return {};

    f32 sqrtD = Math::sqrt(Utility::max(discriminant, 0.0f));
    f32 invA  = 1 / a;
    f32 t0 = (h - sqrtD) * invA;
    f32 t1 = (h + sqrtD) * invA;

    if (t0 > tMax || t1 < tMin)
        return {};

    f32 t = ((t0 >= tMin) ? t0 : t1);
    Vec3 hitP = ray.at(t);
    Vec3 outN = (hitP - spherePos) / radius;
    bool isRayOppositeNormal = (Vec3::dot(ray.direction, outN) < 0.0f);

    f32 phi = Math::arctan2(-outN.z, outN.x);
    if (phi < 0.0f)
        phi += (2 * Math::PI);

    HitResult hitResult {
        .isHit               = true,
        .isRayOppositeNormal = isRayOppositeNormal,
        .hitT                = t,
        .u                   = phi / (2 * Math::PI),
        .v                   = Math::arccos(-outN.y) / Math::PI,
        .hitPoint            = hitP,
        .normal              = (isRayOppositeNormal ? outN : -outN),
        .object              = &sphere
    };

    return hitResult;
}
HitResult Solver::hitTest(const Ray& ray, const Quad& quad, const f32 tMin, const f32 tMax) {
    Vec3 pos = quad.transform().position;
    Vec3 u   = quad.edgeU();
    Vec3 v   = quad.edgeV();
    Vec3 n   = Vec3::cross(u, v);

    f32 denom = Vec3::dot(n, ray.direction);
    if (Math::abs(denom) < EPS_F32)
        return {};

    f32 t = Vec3::dot(n, pos - ray.origin) / denom;
    if (t < tMin || t > tMax)
        return {};

    Vec3 hitP = ray.at(t);
    Vec3 p = hitP - pos;
    Vec3 w = n / n.lengthSquare();

    f32 a = Vec3::dot(w, Vec3::cross(p, v));
    f32 b = Vec3::dot(w, Vec3::cross(u, p));
    if ((a < 0.0f) || (a > 1.0f) || (b < 0.0f) || (b > 1.0f))
        return {};

    Vec3 outN = n.normalized();
    bool isRayOppositeNormal = (Vec3::dot(ray.direction, outN) < 0.0f);

    HitResult hitResult {
        .isHit               = true,
        .isRayOppositeNormal = isRayOppositeNormal,
        .hitT                = t,
        .u                   = a,
        .v                   = b,
        .hitPoint            = hitP,
        .normal              = (isRayOppositeNormal ? outN : -outN),
        .object              = &quad
    };

    return hitResult;
}

bool Solver::isIntersect(const Ray& ray, const AABB& aabb, f32 tMin, f32 tMax) {
    for (u32 axis = 0; axis < 3; ++axis) {
        f32 rayO = ray.origin.data[axis];
        f32 rayD = ray.direction.data[axis];
        f32 axisMin = aabb.min.data[axis];
        f32 axisMax = aabb.max.data[axis];

        if ((rayD * rayD) < 1.0E-12f) {
            if (!Utility::isInClosedRange(rayO, axisMin, axisMax))
                return false;

            continue;
        }

        const f32 invDir = 1 / rayD;

        f32 t0 = (axisMin - rayO) * invDir;
        f32 t1 = (axisMax - rayO) * invDir;

        if (t0 > t1)
            Utility::swap(t0, t1);

        tMin = Utility::max(tMin, t0);
        tMax = Utility::min(tMax, t1);

        if (tMax < tMin)
            return false;
    }

    return true;
}