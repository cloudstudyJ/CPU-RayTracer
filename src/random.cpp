#include "random.hpp"
#include "math.hpp"
#include "utility.hpp"

#include <random>

namespace {
    // [-1, 1]
    Vec3 uniformSphereRange(const f32 zMin, const f32 zMax) {
        const f32 phi = 2 * Math::PI * Random::uniform();
        const f32 z   = zMin + (zMax - zMin) * Random::uniform();
        const f32 a   = Math::sqrt(Utility::max(0.0f, 1 - z * z));

        return {
            a * Math::cos(phi),
            a * Math::sin(phi),
            z
        };
    }
}

namespace Random {
    f32 uniform() {
        thread_local std::mt19937 gen(std::random_device{}());
        thread_local std::uniform_real_distribution<f32> dis(0.0f, 1.0f);

        return dis(gen);
    }

    Vec3 uniformSphere() { return uniformSphereRange(-1.0f, 1.0f); }
    Vec3 uniformHemisphere() { return uniformSphereRange(0.0f, 1.0f); }
    Vec3 cosWeightedHemisphere() {
        const f32 phi       = 2 * Math::PI * uniform();
        const f32 sinTheta2 = uniform();
        const f32 sinTheta  = Math::sqrt(Utility::max(0.0f, sinTheta2));
        const f32 cosTheta  = Math::sqrt(Utility::max(0.0f, 1 - sinTheta2));

        return {
            sinTheta * Math::cos(phi),
            sinTheta * Math::sin(phi),
            cosTheta
        };
    }
}