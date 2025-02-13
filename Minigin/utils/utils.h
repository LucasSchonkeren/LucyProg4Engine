#pragma once

#include <memory>
#include <vector>
#include <set>

//-------------|Typedefs|------------------

// Unique Pointer aliases because typenames get looooong

template<typename T>
using stdUptr = std::unique_ptr<T>;

template <typename T>
using stdUptrVec = std::vector<std::unique_ptr<T>>;

template <typename T>
using stdUptrSet = std::set<std::unique_ptr<T>>;