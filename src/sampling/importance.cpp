#include "sampling/importance.hpp"
#include "sampling/random.hpp"
#include "utility.hpp"

namespace sampling {
    Vec3 cosHemisphere() {
        const f32 phi   = Math::TWO_PI * sampling::uniform();
        const f32 sinSQ = sampling::uniform();
        const f32 sin   = Math::sqrt(Utility::max(0.0f, sinSQ));
        const f32 cos   = Math::sqrt(Utility::max(0.0f, 1 - sinSQ));

        return {
            .data = {
                sin * Math::cos(phi),
                sin * Math::sin(phi),
                cos
            }
        };
    }
}