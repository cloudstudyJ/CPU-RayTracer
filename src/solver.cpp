#include "solver.hpp"
#include "utility.hpp"
#include "objects/sphere.hpp"
#include "objects/quad.hpp"
#include "aabb.hpp"
#include "math.hpp"

HitResult Solver::hitTest(const Ray& ray, const Sphere& sphere, const float32 tMin, const float32 tMax) {
    Vec3 spherePos = sphere.pos();
    float32 radius = sphere.radius();

    Vec3 ac = spherePos - ray.origin;
    float32 a = ray.direction.lengthSquare();
    float32 h = Vec3::dot(ray.direction, ac);
    float32 c = ac.lengthSquare() - radius * radius;

    float32 discriminant = h * h - a * c;
    if (discriminant < -EPS_F32)
        return {};

    float32 sqrtD = Math::sqrt(Utility::max(discriminant, 0.0f));
    float32 invA  = 1 / a;
    float32 t0 = (h - sqrtD) * invA;
    float32 t1 = (h + sqrtD) * invA;

    if (t0 > tMax || t1 < tMin)
        return {};

    float32 t = ((t0 >= tMin) ? t0 : t1);
    Vec3 hitP = ray.at(t);
    Vec3 outN = (hitP - spherePos) / radius;
    bool isRayOppositeNormal = (Vec3::dot(ray.direction, outN) < 0.0f);

    float32 phi = Math::arctan2(-outN.z, outN.x);
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
        .material            = sphere.material()
    };

    return hitResult;
}
HitResult Solver::hitTest(const Ray& ray, const Quad& quad, const float32 tMin, const float32 tMax) {
    Vec3 pos = quad.pos();
    Vec3 u   = quad.edgeU();
    Vec3 v   = quad.edgeV();
    Vec3 n   = Vec3::cross(u, v);

    float32 denom = Vec3::dot(n, ray.direction);
    if (denom > -EPS_F32 && denom < EPS_F32)
        return {};

    float32 t = Vec3::dot(n, pos - ray.origin) / denom;
    if (t < tMin || t > tMax)
        return {};

    Vec3 hitP = ray.at(t);
    Vec3 p = hitP - pos;
    Vec3 w = n / n.lengthSquare();

    float32 a = Vec3::dot(w, Vec3::cross(p, v));
    float32 b = Vec3::dot(w, Vec3::cross(u, p));
    if ((a < 0.0f) || (a > 1.0f) || (b < 0.0f) || (b > 1.0f))
        return {};

    Vec3 outN = n.normalize();
    bool isRayOppositeNormal = (Vec3::dot(ray.direction, outN) < 0.0f);

    HitResult hitResult {
        .isHit               = true,
        .isRayOppositeNormal = isRayOppositeNormal,
        .hitT                = t,
        .u                   = a,
        .v                   = b,
        .hitPoint            = hitP,
        .normal              = (isRayOppositeNormal ? outN : -outN),
        .material            = quad.material()
    };

    return hitResult;
}

bool Solver::isIntersect(const Ray& ray, const AABB& aabb, float32 tMin, float32 tMax) {
    for (uint32 axis = 0; axis < 3; ++axis) {
        float32 rayO = ray.origin.data[axis];
        float32 rayD = ray.direction.data[axis];
        float32 axisMin = aabb.min.data[axis];
        float32 axisMax = aabb.max.data[axis];

        if ((rayD * rayD) < 1.0E-12f) {
            if (!Utility::isInRange(rayO, axisMin, axisMax))
                return false;

            continue;
        }

        const float32 invDir = 1 / rayD;

        float32 t0 = (axisMin - rayO) * invDir;
        float32 t1 = (axisMax - rayO) * invDir;

        if (t0 > t1)
            Utility::swap(t0, t1);

        tMin = Utility::max(tMin, t0);
        tMax = Utility::min(tMax, t1);

        if (tMax < tMin)
            return false;
    }

    return true;
}