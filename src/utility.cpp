#include "utility.hpp"

#include <random>

float32 Utility::random() noexcept {
    thread_local std::mt19937 gen(std::random_device{}());
    thread_local std::uniform_real_distribution<float32> dis(0.0f, 1.0f);

    return dis(gen);
}