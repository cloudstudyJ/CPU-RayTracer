#include "random.hpp"
#include "math.hpp"

#include <random>

namespace Random {
    float32 uniform() {
        thread_local std::mt19937 gen(std::random_device{}());
        thread_local std::uniform_real_distribution<float32> dis(0.0f, 1.0f);

        return dis(gen);
    }

    Vec3 uniformSphere() {
        const float32 phi = 2 * Math::PI * uniform();
        const float32 z   = 1 - 2 * uniform();
        const float32 a   = Math::sqrt(1 - z * z);

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