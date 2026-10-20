#include "sampling/surface.hpp"
#include "sampling/geometry.hpp"
#include "math/math.hpp"
#include "objects/sphere.hpp"
#include "objects/rectangle.hpp"

namespace sampling {
    SurfaceSample surface(const Sphere& sphere) {
        const f32  radius    = sphere.radius();
        const f32  invRadius = 1 / radius;
        const Vec3 offset    = sampling::uniformSphere(radius);

        return {
            .point  = sphere.transform().position + offset,
            .normal = offset * invRadius,
            .pdf    = (invRadius * invRadius) * Math::INV_FOUR_PI
        };
    }
    SurfaceSample surface(const Rectangle& rectangle) {
        const Vec3 edgeU = rectangle.edgeU();
        const Vec3 edgeV = rectangle.edgeV();

        const Vec3 offset  = sampling::uniformRectangle(edgeU, edgeV);
        const Vec3 normal  = Vec3::cross(edgeU, edgeV);
        const f32  invArea = 1 / normal.length();

        return {
            .point  = rectangle.transform().position + offset,
            .normal = normal * invArea,
            .pdf    = invArea
        };
    }
}