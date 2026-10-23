#include "components/light.hpp"
#include "math/math.hpp"
#include "objects/sphere.hpp"
#include "objects/rectangle.hpp"

f32 surfacePDF(const Rectangle& rectangle) {
    const f32 area = Vec3::cross(
        rectangle.edgeU(),
        rectangle.edgeV()
    ).length();

    return 1 / area;
}
f32 surfacePDF(const Sphere& sphere) {
    const f32 invRadius = 1 / sphere.radius();

    return Math::INV_FOUR_PI * invRadius * invRadius;
}
