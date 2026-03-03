#include "solver.hpp"

#include "vec3.hpp"
#include "math.hpp"
#include "utility.hpp"

HitResult Solver::hitTest(const Ray& ray, const Sphere& sphere) noexcept {
    HitResult hitResult;

    Vec3 ac = sphere.centerPos() - ray.origin();
    Vec3 rayDir = ray.direction();

    float32 a = rayDir.dot(rayDir);
    float32 h = ac.dot(rayDir);
    float32 c = ac.dot(ac) - sphere.radius() * sphere.radius();
    float32 discriminant = h * h - a * c;

    if (discriminant < -Math::EPSILON)
        return hitResult;

    float32 sqrtD = Math::sqrt(Utility::max(discriminant, 0.0f));
    float32 invA = 1 / a;

    float32 t0 = (h - sqrtD) * invA;
    float32 t1 = (h + sqrtD) * invA;

    hitResult.tMin = Utility::min(t0, t1);
    hitResult.tMax = Utility::max(t0, t1);

    if (hitResult.tMax <= T_MIN)
        return hitResult;

    hitResult.isHit  = true;
    hitResult.object = &sphere;

    return hitResult;
}