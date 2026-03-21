#include "random.hpp"
#include "math.hpp"

#include <random>

namespace Random {
    f32 uniform() {
        thread_local std::mt19937 gen(std::random_device{}());
        thread_local std::uniform_real_distribution<f32> dis(0.0f, 1.0f);

        return dis(gen);
    }

    Vec3 uniformSphere() {
        const f32 phi = 2 * Math::PI * uniform();
        const f32 z   = 1 - 2 * uniform();
        const f32 a   = Math::sqrt(1 - z * z);

        return {
            a * Math::cos(phi),
            a * Math::sin(phi),
            z
        };
    }
    Vec3 uniformHemisphere(const Vec3& n) {
        Vec3 sample = uniformSphere();

        return ((Vec3::dot(n, sample) < 0.0f) ? -sample : sample);
    }
}