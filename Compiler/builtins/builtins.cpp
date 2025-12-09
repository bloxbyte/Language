#include "Builtins.h"
#include "console/Console.h"
#include "random/Random.h"
#include "math/math.h"
#include "string/String.h"
#include "system/System.h"
#include "file/File.h"
#include "../utils/Error.h"

BuiltinRegistry& BuiltinRegistry::instance() {
    static BuiltinRegistry reg;
    return reg;
}

void BuiltinRegistry::registerFunction(const String& name, BuiltinFunction func) {
    functions[name] = func;
}

bool BuiltinRegistry::hasFunction(const String& name) const {
    return functions.find(name) != functions.end();
}

Value BuiltinRegistry::callFunction(const String& name, const Vec<Value>& args) const {
    auto it = functions.find(name);
    if (it == functions.end()) {
        throw RuntimeError("Unknown built-in function: " + name);
    }
    return it->second(args);
}

void BuiltinRegistry::registerAll() {
    registerFunction("console.print", Builtins::Console::print);
    registerFunction("console.write", Builtins::Console::write);
    registerFunction("console.error", Builtins::Console::error);

    registerFunction("random.int", Builtins::Random::randomInt);
    registerFunction("random.float", Builtins::Random::randomFloat);

    registerFunction("math.abs", Builtins::Math::abs);
    registerFunction("math.min", Builtins::Math::min);
    registerFunction("math.max", Builtins::Math::max);
    registerFunction("math.pow", Builtins::Math::pow);
    registerFunction("math.sqrt", Builtins::Math::sqrt);
    registerFunction("math.floor", Builtins::Math::floor);
    registerFunction("math.ceil", Builtins::Math::ceil);

    registerFunction("string.length", Builtins::String::length);
    registerFunction("string.substring", Builtins::String::substring);
    registerFunction("string.upper", Builtins::String::toupper);
    registerFunction("string.lower", Builtins::String::tolower);
    registerFunction("string.contains", Builtins::String::contains);
    registerFunction("string.replace", Builtins::String::replace);
    registerFunction("string.split", Builtins::String::split);
    registerFunction("string.trim", Builtins::String::trim);

    registerFunction("system.exit", Builtins::System::exit);
    registerFunction("system.pause", Builtins::System::pause);
    registerFunction("system.version", Builtins::System::version);

    registerFunction("file.read", Builtins::File::read);
    registerFunction("file.write", Builtins::File::write);
    registerFunction("file.create", Builtins::File::create);
    registerFunction("file.exists", Builtins::File::exists);
}