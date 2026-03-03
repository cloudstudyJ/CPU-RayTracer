#include "solver.hpp"

#include "vec3.hpp"
#include "math.hpp"
#include "utility.hpp"

HitResult Solver::hitTest(const Ray& ray, const Sphere& sphere) noexcept {
    HitResult hitResult;

    Vec3 ac = sphere.centerPos() - ray.origin();

    float32 a = ray.direction().dot(ray.direction());
    float32 b = -2 * ray.direction().dot(ac);
    float32 c = ac.dot(ac) - sphere.radius() * sphere.radius();
    float32 discriminant = b * b - 4 * a * c;

    if (discriminant < -Math::EPSILON)
        return hitResult;

    float32 sqrtD = Math::sqrt(Utility::max(discriminant, 0.0f));
    float32 inv2a = 1 / (2 * a);

    float32 t0 = (-b - sqrtD) * inv2a;
    float32 t1 = (-b + sqrtD) * inv2a;

    hitResult.tIn  = Utility::min(t0, t1);
    hitResult.tOut = Utility::max(t0, t1);

    if (hitResult.tOut < 0.001f)
        return hitResult;

    hitResult.isHit  = true;
    hitResult.object = &sphere;

    return hitResult;
}