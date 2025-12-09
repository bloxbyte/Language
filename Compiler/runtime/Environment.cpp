#include "Environment.h"
#include "../utils/Error.h"

Environment::Environment(Ptr<Environment> enc) : enclosing(enc) {}

void Environment::define(const String& name, const Value& value) {
    values[name] = value;
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
    auto it = values.find(name);
    if (it != values.end()) {
        it->second = value;
        return;
    }

    if (enclosing) {
        enclosing->set(name, value);
        return;
    }

    throw NameError("Undefined variable '" + name + "'");
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