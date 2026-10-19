#include "sampling/random.hpp"

#include <random>

namespace sampling {
    // [0.0f, 1.0f)
    f32 uniform() {
        thread_local std::mt19937 gen(std::random_device{}());
        thread_local std::uniform_real_distribution<f32> dis(0.0f, 1.0f);

        return dis(gen);
    }
    // [0.0f, 1.0f)^2
    Vec2 uniform2D() {
        return {
            .data = {
                uniform(),
                uniform()
            }
        };
    }

    // [from, to)
    f32 uniform(const f32 from, const f32 to) {
        return from + (to - from) * uniform();
    }
    // [from, to)^2
    Vec2 uniform2D(const f32 from, const f32 to) {
        return {
            .data = {
                uniform(from, to),
                uniform(from, to)
            }
        };
    }
}