#pragma once

#include "aabb.hpp"
#include "object.hpp"
#include "hitResult.hpp"
#include "solver.hpp"

struct BVHNode {
    public:
        BVHNode() noexcept: left(nullptr), right(nullptr), object(nullptr) {}

        HitResult hitTest(const Ray& ray, const float32 tMin, const float32 tMax) const {
            if (!Solver::isIntersect(ray, aabb, tMin, tMax))
                return {};

            if (object != nullptr)
                return object->hitTest(ray, tMin, tMax);

            HitResult leftHitResult  = ((left != nullptr) ? left->hitTest(ray, tMin, tMax) : HitResult{});
            HitResult rightHitResult = ((right != nullptr) ? right->hitTest(ray, tMin, (leftHitResult.isHit ? leftHitResult.hitT : tMax)) : HitResult{});

            if (leftHitResult.isHit && rightHitResult.isHit)
                return ((leftHitResult.hitT < rightHitResult.hitT) ? leftHitResult : rightHitResult);

            if (leftHitResult.isHit)       return leftHitResult;
            else if (rightHitResult.isHit) return rightHitResult;

            return {};
        }

    public:
        AABB aabb;

        BVHNode* left;
        BVHNode* right;

        Object* object;
};