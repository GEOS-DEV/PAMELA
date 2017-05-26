#pragma once

// Std library includes
#include <type_traits>
#include <unordered_map>

struct EnumHash
{
  template <typename T>
  std::size_t operator()(T t) const
  {
    return static_cast<std::size_t>(t);
  }
};

template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumHash, std::hash<Key>>::type;

template <typename Key, typename T>
using HashMap = std::unordered_map<Key, T, HashType<Key>>;

template <typename Key, typename T>
using HashMultiMap = std::unordered_multimap<Key, T, HashType<Key>>;
