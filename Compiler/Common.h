#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <variant>
#include <stdexcept>

class ASTNode;
class Value;
class Environment;

using String = std::string;
template<typename T>
using Vec = std::vector<T>;
template<typename T>
using Ptr = std::shared_ptr<T>;
template<typename K, typename V>
using Map = std::map<K, V>;

using ValueData = std::variant<int, String, bool>;

namespace Constants {
    constexpr const char* VERSION = "0.0.3";
    constexpr int MAX_RECURSION_DEPTH = 1000;
}

#define MAKE_PTR(T, ...) std::make_shared<T>(__VA_ARGS__)