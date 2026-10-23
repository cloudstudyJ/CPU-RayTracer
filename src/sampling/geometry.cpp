#include "sampling/geometry.hpp"
#include "sampling/random.hpp"
#include "utility.hpp"

// [-1, 1]
Vec3 sampleSphericalBand(const f32 zMin, const f32 zMax) {
    const f32 phi = Math::TWO_PI * sampling::uniform();
    const f32 z   = zMin + (zMax - zMin) * sampling::uniform();
    const f32 a   = Math::sqrt(Utility::max(0.0f, 1 - z * z));

    return {
        a * Math::cos(phi),
        a * Math::sin(phi),
        z
    };
}

namespace sampling {
    Vec2 uniformDisk(const f32 radius) {
        const Vec2 uv  = sampling::uniform2D();
        const f32  phi = Math::TWO_PI * uv.u;
        const f32  r   = Math::sqrt(uv.v) * radius;

        return {
            r * Math::cos(phi),
            r * Math::sin(phi)
        };
    }
    Vec3 uniformSphere(const f32 radius) {
        return sampleSphericalBand(-1.0f, 1.0f) * radius;
    }
    Vec3 uniformHemisphere(const f32 radius) {
        return sampleSphericalBand(0.0f, 1.0f) * radius;
    }
    Vec3 uniformRectangle(const Vec3& edgeU, const Vec3& edgeV) {
        const Vec2 uv = sampling::uniform2D(-0.5f, 0.5f);

        return ((edgeU * uv.u) + (edgeV * uv.v));
    }
}