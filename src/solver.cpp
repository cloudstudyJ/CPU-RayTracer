#include "solver.hpp"
#include "utility.hpp"
#include "objects/sphere.hpp"
#include "objects/rectangle.hpp"
#include "aabb.hpp"
#include "math/math.hpp"
#include "ray.hpp"

HitResult Solver::hitTest(const Ray& ray, const Sphere& sphere, const f32 tMin, const f32 tMax) {
    const Vec3 spherePos = sphere.transform().position;
    const f32  radius    = sphere.radius();

    Vec3 ac = spherePos - ray.origin;
    f32  a  = ray.direction.lengthSquare();
    f32  h  = Vec3::dot(ray.direction, ac);
    f32  c  = ac.lengthSquare() - radius * radius;

    f32 discriminant = h * h - a * c;
    if (discriminant < -EPS_F32)
        return {};

    f32 sqrtD = Math::sqrt(Utility::max(0.0f, discriminant));
    f32 invA  = 1 / a;
    f32 t0    = (h - sqrtD) * invA;
    f32 t1    = (h + sqrtD) * invA;

    if (t0 > tMax || t1 < tMin)
        return {};

    f32  t = ((t0 >= tMin) ? t0 : t1);

    if (t > tMax)
        return {};

    Vec3 hitP        = ray.at(t);
    Vec3 outN        = (hitP - spherePos) / radius;
    bool isFrontFace = (Vec3::dot(ray.direction, outN) < 0.0f);

    f32 phi = Math::arctan2(-outN.z, outN.x);
    if (phi < 0.0f)
        phi += Math::TWO_PI;

    HitResult hitResult {
        .isHit       = true,
        .isFrontFace = isFrontFace,
        .hitT        = t,
        .uv          = {
            .u = phi * Math::INV_TWO_PI,
            .v = Math::arccos(-outN.y) * Math::INV_PI
        },
        .hitPoint    = hitP,
        .normal      = (isFrontFace ? outN : -outN),
        .object      = &sphere
    };

    return hitResult;
}
HitResult Solver::hitTest(const Ray& ray, const Rectangle& rect, const f32 tMin, const f32 tMax) {
    const Vec3 pos = rect.transform().position;
    const Vec3 u   = rect.edgeU();
    const Vec3 v   = rect.edgeV();
    const Vec3 n   = Vec3::cross(u, v);

    f32 denom = Vec3::dot(n, ray.direction);
    if (Math::abs(denom) < EPS_F32)
        return {};

    f32 t = Vec3::dot(n, pos - ray.origin) / denom;
    if (t < tMin || t > tMax)
        return {};

    Vec3 hitP = ray.at(t);
    Vec3 p    = hitP - pos;
    Vec3 w    = n / n.lengthSquare();

    f32 a = Vec3::dot(w, Vec3::cross(p, v));
    f32 b = Vec3::dot(w, Vec3::cross(u, p));
    if ((a < -0.5f) || (a > 0.5f) || (b < -0.5f) || (b > 0.5f))
        return {};

    Vec3 outN        = n.normalized();
    bool isFrontFace = (Vec3::dot(ray.direction, outN) < 0.0f);

    HitResult hitResult {
        .isHit       = true,
        .isFrontFace = isFrontFace,
        .hitT        = t,
        .uv          = {
            .u = a + 0.5f,
            .v = b + 0.5f
        },
        .hitPoint    = hitP,
        .normal      = (isFrontFace ? outN : -outN),
        .object      = &rect
    };

    return hitResult;
}

bool Solver::isIntersect(const Ray& ray, const AABB& aabb, f32 tMin, f32 tMax) {
    for (u32 axis = 0; axis < 3; ++axis) {
        const f32 rayO    = ray.origin.data[axis];
        const f32 rayD    = ray.direction.data[axis];
        const f32 axisMin = aabb.min.data[axis];
        const f32 axisMax = aabb.max.data[axis];

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