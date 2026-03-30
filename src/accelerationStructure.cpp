#include "accelerationStructure.hpp"
#include "objects/object.hpp"
#include "ray.hpp"
#include "solver.hpp"

#include <algorithm>

namespace {
    u32 longestAxis(const AABB& aabb) {
        const f32 xLength = aabb.max.x - aabb.min.x;
        const f32 yLength = aabb.max.y - aabb.min.y;
        const f32 zLength = aabb.max.z - aabb.min.z;

        if (xLength > yLength) return ((xLength > zLength) ? 0 : 2);
        else                   return ((yLength > zLength) ? 1 : 2);
    }

    // [from, to)
    AABB buildAABB(const std::vector<Object*>& objRefs, const u32 from, const u32 to) {
        AABB result;

        for (u32 i = from; i < to; ++i)
            result = AABB::unite(result, objRefs[i]->aabb());

        return result;
    }
}

AccelerationStructure::AccelerationStructure(AccelerationStructure&& other) noexcept
    : mRoot(std::move(other.mRoot)) { other.mRoot = {}; }
AccelerationStructure::~AccelerationStructure() noexcept { clear(); }

AccelerationStructure& AccelerationStructure::operator=(AccelerationStructure&& other) noexcept {
    if (this == &other)
        return *this;

    clear();

    mRoot       = std::move(other.mRoot);
    other.mRoot = {};

    return *this;
}

// [from, to)
void AccelerationStructure::build(const std::vector<std::unique_ptr<Object>>& objects) {
    std::vector<Object*> objRefs;
    objRefs.reserve(objects.size());

    for (const auto& obj: objects)
        objRefs.emplace_back(obj.get());

    mRoot = build(objRefs, 0, static_cast<u32>(objects.size()));
}
void AccelerationStructure::clear() noexcept {
    if (!mRoot)
        return;

    Node* stk[128];

    u32 ptr{};
    stk[ptr++] = mRoot;

    while (ptr) {
        Node* node = stk[--ptr];

        if (node->left)  stk[ptr++] = node->left;
        if (node->right) stk[ptr++] = node->right;

        delete node;
    }

    mRoot = {};
}

HitResult AccelerationStructure::hitTest(const Ray& ray, const f32 tMin, const f32 tMax) noexcept {
    Node* stk[128];

    u32 ptr{};
    stk[ptr++] = mRoot;

    HitResult hitResult;
    hitResult.hitT = tMax;

    while (ptr) {
        Node* node = stk[--ptr];

        if (!Solver::isIntersect(ray, node->aabb, tMin, hitResult.hitT))
            continue;

        if (node->obj) {
            HitResult result = node->obj->hitTest(ray, tMin, hitResult.hitT);

            if (result.isHit)
                hitResult = result;

            continue;
        }

        Node* l = node->left;
        Node* r = node->right;

        if (r && Solver::isIntersect(ray, r->aabb, tMin, hitResult.hitT)) stk[ptr++] = r;
        if (l && Solver::isIntersect(ray, l->aabb, tMin, hitResult.hitT)) stk[ptr++] = l;
    }

    return hitResult;
}

// [from, to)
AccelerationStructure::Node* AccelerationStructure::build(std::vector<Object*>& objRefs, const u32 from, const u32 to) {
    Node* newNode = new Node;
    newNode->aabb = buildAABB(objRefs, from, to);

    const u32 nodeCount = to - from;
    if (nodeCount == 1) {
        newNode->obj = objRefs[from];
        return newNode;
    }

    const u32 axis = longestAxis(newNode->aabb);
    const u32 midIdx = from + (nodeCount >> 1);

    std::nth_element(
        objRefs.begin() + from,
        objRefs.begin() + midIdx,
        objRefs.begin() + to,
        [axis](const Object* a, const Object* b) noexcept -> bool {
            return (a->aabb().min.data[axis] < b->aabb().min.data[axis]);
        }
    );

    newNode->left  = build(objRefs, from, midIdx);
    newNode->right = build(objRefs, midIdx, to);

    return newNode;
}