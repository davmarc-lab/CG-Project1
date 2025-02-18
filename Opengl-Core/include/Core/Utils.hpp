#pragma once

template <typename T>
struct Pair {
	T x, y;

    friend bool operator==(const Pair<T>& lhs, const Pair<T>& rhs) {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }
};

#include <memory>

template <typename T>
using Unique = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Unique<T> CreateUnique(Args &&...args) { return std::make_unique<T>(std::forward<Args>(args)...); }

template <typename T>
using Shared = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Shared<T> CreateShared(Args &&...args) { return std::make_shared<T>(std::forward<Args>(args)...); }

#include <cassert>

// MACROS

#define ASSERT(exp) assert(exp)

// `container.begin(), container.end()`
#define ALL(a) (a.begin()), (a.end())

// Class name
#define CLASSNAME(elem) typeid(elem).name()
