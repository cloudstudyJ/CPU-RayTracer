#pragma once

#include "types.hpp"

namespace sampling {
    f32 powerHeuristic(const f32 pdfA, const f32 pdfB) noexcept;
    f32 balancedHeuristic(const f32 pdfA, const f32 pdfB) noexcept;
}
