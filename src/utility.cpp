#include "utility.hpp"

std::random_device Utility::rd;
std::mt19937       Utility::gen(rd());

float32 Utility::random() noexcept {
    static std::uniform_real_distribution<float32> dis(0.0f, 1.0f);

    return dis(gen);
}