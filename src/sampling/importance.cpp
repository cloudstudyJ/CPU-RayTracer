#include "sampling/importance.hpp"
#include "sampling/random.hpp"
#include "utility.hpp"

namespace sampling {
    Vec3 cosHemisphere() {
        const Vec2 uv  = sampling::uniform2D();
        const f32  phi = Math::TWO_PI * uv.u;
        const f32  r   = Math::sqrt(uv.v);

        return {
            r * Math::cos(phi),
            r * Math::sin(phi),
            Math::sqrt(Utility::max(0.0f, 1 - uv.v))
        };
    }
}