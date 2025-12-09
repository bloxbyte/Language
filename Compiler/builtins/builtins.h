#pragma once

#include "../Common.h"
#include "../runtime/Value.h"
#include <functional>

using BuiltinFunction = std::function<Value(const Vec<Value>&)>;

class BuiltinRegistry {
public:
    static BuiltinRegistry& instance();

    void registerFunction(const String& name, BuiltinFunction func);
    bool hasFunction(const String& name) const;
    Value callFunction(const String& name, const Vec<Value>& args) const;

    void registerAll();

private:
    BuiltinRegistry() = default;
    Map<String, BuiltinFunction> functions;
};