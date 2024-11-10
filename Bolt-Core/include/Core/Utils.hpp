#pragma once

#include <memory>

// --- MACROS ---

// Math
#define BTMIN(a, b) ((a) < (b) ? (a) : (b))
// #define BTMININC(a, b)	((a) <= (b) ? (a) : (b))
#define BTMAX(a, b) ((a) > (b) ? (a) : (b))
// #define BTMAXINC(a, b)	((a) >= (b) ? (a) : (b))

// constexpression
#define BT_CONSTEXPR constexpr
#define BT_AUTOCONST const auto

// --- ALIAS ---

// Base Types
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using i8 = char;
using i16 = short;
using i32 = int;
using i64 = long long;

// should be better
using b8 = bool;

using f32 = float;
using f64 = double;

/*
 * Alias for `std::unique_ptr<T>`
 *
 * @tparam T type of the stored value.
 */
template <typename T>
using Unique = std::unique_ptr<T>;

/*
 * Alias for `std::make_unique<T>(std::forward<Args>(args)...)`
 *
 * @tparam T type of the created value.
 * @tparam Args... all optional arguments
 *
 * @return an `unique_ptr` of type T, using the constructor with `Args...` parameters.
 */
template <typename T, typename... Args>
constexpr Unique<T> CreateUnique(Args&&... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

/*
 * Alias for `std::shared_ptr<T>`
 *
 * @tparam T type of the stored value.
 */
template <typename T>
using Shared = std::shared_ptr<T>;

/*
 * Alias for `std::make_shared<T>(std::forward<Args>(args)...)`
 *
 * @tparam T type of the created value.
 * @tparam Args... all optional arguments
 *
 * @return a `shared_ptr` of type T, using the constructor with `Args...` parameters.
 */
template <typename T, typename... Args>
constexpr Shared<T> CreateShared(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

// Check errors

#define STATIC_ASSERT static_assert

STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");

STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");

STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");

STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

#undef STATIC_ASSERT
