#include "utility.hpp"

#include <random>

static std::mt19937 gen(std::random_device{}());

float32 Utility::random() noexcept {
    static std::uniform_real_distribution<float32> dis(0.0f, 1.0f);

    return dis(gen);
}