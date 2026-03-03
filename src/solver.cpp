#include "solver.hpp"

#include "vec3.hpp"
#include "math.hpp"

HitResult Solver::hitTest(const Ray& ray, const Sphere& sphere) noexcept {
    HitResult hitResult;

    Vec3 ac = sphere.centerPos() - ray.origin();

    float32 a = ray.direction().dot(ray.direction());
    float32 b = -2 * ray.direction().dot(ac);
    float32 c = ac.dot(ac) - sphere.radius() * sphere.radius();
    float32 discriminant = b * b - 4 * a * c;

    hitResult.isHit = (discriminant >= -Math::EPSILON);

    return hitResult;
}