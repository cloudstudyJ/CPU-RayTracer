#include "sampling/mis.hpp"

namespace sampling {
    f32 powerHeuristic(const f32 pdfA, const f32 pdfB) noexcept {
        const f32 aSQ = pdfA * pdfA;
        const f32 bSQ = pdfB * pdfB;

        return aSQ / (aSQ + bSQ);
    }
    f32 balancedHeuristic(const f32 pdfA, const f32 pdfB) noexcept {
        return pdfA / (pdfA + pdfB);
    }
}
