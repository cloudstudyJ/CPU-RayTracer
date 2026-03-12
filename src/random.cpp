#include "random.hpp"
#include "math.hpp"

#include <random>

namespace Random {
    float32 uniform() {
        thread_local std::mt19937 gen(std::random_device{}());
        thread_local std::uniform_real_distribution<float32> dis(0.0f, 1.0f);

        return dis(gen);
    }
    Vec3 uniformVec3(const float32 from, const float32 to) {
        return {
            uniform(from, to),
            uniform(from, to),
            uniform(from, to)
        };
    }

    // rejection method
    Vec3 uniformSphere() {
        while (true) {
            Vec3    sample = uniformVec3(-1.0f, 1.0f);
            float32 len2   = sample.lengthSquare();

            if ((len2 > 0.0f) && (len2 <= 1.0f))
                return (sample / Math::sqrt(len2));
        }
    }
    Vec3 uniformHemisphere(const Vec3& normal) {
        Vec3 sample = uniformSphere();

        if (Vec3::dot(normal, sample) <= 0.0f)
            sample = -sample;

        return sample;
    }
}