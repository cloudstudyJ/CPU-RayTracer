#pragma once

#include "ray.hpp"

struct SampleResult {
    Ray  wo;
    Vec3 albedo;
};