#pragma once

#include "SdbmHash.h"

#include <memory>
#include <vector>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <optional>
#include <ranges>

//------------------|Defines|-----------------

#define val_get() value().get()


//-------------|Typedefs|------------------

// Unique Pointer aliases because typenames get looooong

template<typename T>
using u_ptr = std::unique_ptr<T>;

template <typename T>
using u_ptr_vec = std::vector<std::unique_ptr<T>>;

template <typename T>
using u_ptr_set = std::set<std::unique_ptr<T>>;

template <typename T>
using ref_vec = std::vector<std::reference_wrapper<T>>;

template <typename T>
using ref_set = std::set<std::reference_wrapper<T>, std::less<T>>;

template <typename T>
using type_map_unordered = std::unordered_map<std::type_index, T>;

template <typename T>
using u_ptr_type_map_unordered = std::unordered_map<std::type_index, std::unique_ptr<T>>;

//--------------------|Concepts|-----------------------

template <typename R, typename V>
concept RangeOf = std::ranges::range<R> && std::same_as<std::ranges::range_value_t<R>, V>;

//--------------------|Engine Event Hashes|-------------

/// Predefined event hashes for engine-wide events, to avoid possible misspellings of strings in hash functions and to ensure all hashes are unique

namespace eng::eventHash {

constexpr unsigned int actorResourceChanged{ make_sdbm_hash("ActorResourceChanged") };

}