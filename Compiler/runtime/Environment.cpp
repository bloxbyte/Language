#include "Environment.h"

Environment::Environment(Ptr<Environment> enc) : enclosing(enc) {}

void Environment::define(const String& name, const Value& value) {
    variables[name] = value;
    variableCache[name] = &variables[name];
}

Value Environment::get(const String& name) const {
    auto it = values.find(name);
    if (it != values.end()) {
        return it->second;
    }

    if (enclosing) {
        return enclosing->get(name);
    }

    throw NameError("Undefined variable '" + name + "'");
}

void Environment::set(const String& name, const Value& value) {
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

bool Environment::exists(const String& name) const {
    if (values.find(name) != values.end()) {
        return true;
    }
    return enclosing && enclosing->exists(name);
}

void Environment::defineFunction(const String& name, Ptr<FuncDefinitionNode> func) {
    functions[name] = func;
}

Ptr<FuncDefinitionNode> Environment::getFunction(const String& name) const {
    auto it = functions.find(name);
    if (it != functions.end()) {
        return it->second;
    }

    if (enclosing) {
        return enclosing->getFunction(name);
    }

    return nullptr;
}

bool Environment::hasFunction(const String& name) const {
    if (functions.find(name) != functions.end()) {
        return true;
    }
    return enclosing && enclosing->hasFunction(name);
}

void Environment::defineStruct(const String& name, Ptr<StructDefinitionNode> structDef) {
    structs[name] = structDef;
}

Ptr<StructDefinitionNode> Environment::getStruct(const String& name) const {
    auto it = structs.find(name);
    if (it != structs.end()) {
        return it->second;
    }

    if (enclosing) {
        return enclosing->getStruct(name);
    }

    return nullptr;
}

bool Environment::hasStruct(const String& name) const {
    if (structs.find(name) != structs.end()) {
        return true;
    }
    return enclosing && enclosing->hasStruct(name);
}