#pragma once

#include "SdbmHash.h"

#include <memory>
#include <vector>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <optional>
#include <ranges>
#include <string>
#include <map>

//-------------|Macros|------------------

// String/enum macros to easily create enum/string switch cases

#define ENUM_TOSTRING_DECL(x) ::std::string ToString(x arg);
#define ENUM_TOSTRING_START ::std::string ToString(ENUM_NAME arg) { switch (arg) {
#define ENUM_TOSTRING_CASE(arg) case ENUM_NAME##::arg: return #arg;
#define ENUM_TOSTRING_END default: return "";}} 

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
/// <summary>
/// 
/// </summary>
constexpr unsigned int actorResourceChanged{ make_sdbm_hash("ActorResourceChanged") };
/// <summary>
/// 
/// </summary>
constexpr unsigned int PositionChanged{ make_sdbm_hash("PositionChanged") };


}

//--------------------|Hash functions|------------------

// A hash function used to hash a pair of any kind. Taken from https://www.geeksforgeeks.org/how-to-create-an-unordered_map-of-pairs-in-c/
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        // Hash the first element
        size_t hash1 = hash<T1>()(p.first);
        // Hash the second element
        size_t hash2 = hash<T2>()(p.second);
        // Combine the two hash values
        return hash1
            ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
            + (hash1 >> 2));
    }
};
