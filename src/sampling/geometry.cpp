#include "sampling/geometry.hpp"
#include "sampling/random.hpp"
#include "utility.hpp"

// [-1, 1]
Vec3 sampleSphericalBand(const f32 zMin, const f32 zMax) noexcept {
    const f32 phi = Math::TWO_PI * sampling::uniform();
    const f32 z   = zMin + (zMax - zMin) * sampling::uniform();
    const f32 a   = Math::sqrt(Utility::max(0.0f, 1 - z * z));

    return {
        .data = {
            a * Math::cos(phi),
            a * Math::sin(phi),
            z
        }
    };
}

namespace sampling {
    Vec3 uniformSphere() {
        return sampleSphericalBand(-1.0f, 1.0f);
    }
    Vec3 uniformHemisphere() {
        return sampleSphericalBand(0.0f, 1.0f);
    }
}