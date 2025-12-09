#pragma once
#pragma once

#include "../Common.h"
#include "Value.h"
#include "../parser/AST.h"

class Environment {
public:
    explicit Environment(Ptr<Environment> enclosing = nullptr);

    void define(const String& name, const Value& value);
    Value get(const String& name) const;
    void set(const String& name, const Value& value);
    bool exists(const String& name) const;

    void defineFunction(const String& name, Ptr<FuncDefinitionNode> func);
    Ptr<FuncDefinitionNode> getFunction(const String& name) const;
    bool hasFunction(const String& name) const;

    void defineStruct(const String& name, Ptr<StructDefinitionNode> structDef);
    Ptr<StructDefinitionNode> getStruct(const String& name) const;
    bool hasStruct(const String& name) const;

    Ptr<Environment> getEnclosing() const { return enclosing; }

    Value get(const String& name) {
        auto cacheIt = variableCache.find(name);
        if (cacheIt != variableCache.end()) {
            return *(cacheIt->second);
        }

        auto it = variables.find(name);
        if (it != variables.end()) {
            variableCache[name] = &(it->second);
            return it->second;
        }

        if (parent) {
            return parent->get(name);
        }

        throw NameError("Undefined variable: " + name);
    }

    void set(const String& name, const Value& value) {
        auto it = variables.find(name);
        if (it != variables.end()) {
            it->second = value;
            auto cacheIt = variableCache.find(name);
            if (cacheIt != variableCache.end()) {
                cacheIt->second = &(it->second);
            }
            return;
        }

        if (parent) {
            parent->set(name, value);
            return;
        }

        throw NameError("Undefined variable: " + name);
    }

    void define(const String& name, const Value& value) {
        variables[name] = value;
        variableCache[name] = &variables[name];
    }

    void clearCache() {
        variableCache.clear();
    }

private:
    Map<String, Value> variables;
    Ptr<Environment> parent;

    mutable Map<String, Value*> variableCache;

    Map<String, Value> values;
    Map<String, Ptr<FuncDefinitionNode>> functions;
    Map<String, Ptr<StructDefinitionNode>> structs;
    Ptr<Environment> enclosing;
};