#include "sphere.hpp"
#include "constants.hpp"
#include "math.hpp"
#include "utility.hpp"

HitResult Sphere::hitTest(const Ray& ray, const float32 tMin, const float32 tMax) {
    HitResult hitResult;

    Vec3 ac = mPos - ray.origin();
    Vec3 rayDir = ray.direction();

    float32 a = rayDir.dot(rayDir);
    float32 h = ac.dot(rayDir);
    float32 c = ac.dot(ac) - mRadius * mRadius;

    float32 discriminant = h * h - a * c;
    if (discriminant < -EPS_F32)
        return hitResult;

    float32 sqrtD = Math::sqrt(Utility::max(discriminant, 0.0f));
    float32 invA  = 1 / a;

    float32 t0 = (h - sqrtD) * invA;
    float32 t1 = (h + sqrtD) * invA;
    if (t0 > t1)
        Utility::swap(t0, t1);

    hitResult.hitT = t0;
    if (hitResult.hitT < tMin || hitResult.hitT > tMax) {
        hitResult.hitT = t1;

        if (hitResult.hitT < tMin || hitResult.hitT > tMax)
            return hitResult;
    }

    hitResult.isHit    = true;
    hitResult.hitPoint = ray.at(hitResult.hitT);
    hitResult.normal   = (hitResult.hitPoint - mPos) / mRadius;

    return hitResult;
}