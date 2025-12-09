#include "system.h"
#include "../../utils/Error.h"
#include <iostream>
#include <cstdlib>

namespace Builtins {
    namespace System {
        Value exit(const Vec<Value>& args) {
            int code = 0;

            if (!args.empty()) {
                if (!args[0].isInt()) {
                    throw TypeError("system.exit() requires integer argument");
                }
                code = args[0].asInt();
            }

            std::exit(code);
            return Value::makeNil();
        }

        Value pause(const Vec<Value>& args) {
            std::cout << "Press Enter to continue...";
            std::cout.flush();
            std::cin.get();
            return Value::makeNil();
        }

        Value version(const Vec<Value>& args) {
            return Value::makeString(Constants::VERSION);
        }
    } 
} 