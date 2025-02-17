#pragma once

#include <memory>
#include <vector>
#include <set>
#include <typeindex>
#include <unordered_map>

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
using type_map_unordered = std::unordered_map<std::type_index, T>;

template <typename T>
using u_ptr_type_map_unordered = std::unordered_map<std::type_index, std::unique_ptr<T>>;

template <typename T>
using optional_ref = std::optional<std::reference_wrapper<T>>;