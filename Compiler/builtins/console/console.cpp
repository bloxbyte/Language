#include "console.h"
#include "../../utils/Error.h"
#include <iostream>

namespace Builtins {
    namespace Console {

        Value print(const Vec<Value>& args) {
            if (args.empty()) {
                std::cout << std::endl;
                return Value::makeNil();
            }

            for (size_t i = 0; i < args.size(); ++i) {
                if (i > 0) std::cout << " ";
                std::cout << args[i].toString();
            }
            std::cout << std::endl;

            return Value::makeNil();
        }

        Value write(const Vec<Value>& args) {
            for (size_t i = 0; i < args.size(); ++i) {
                if (i > 0) std::cout << " ";
                std::cout << args[i].toString();
            }
            std::cout << std::flush;

            return Value::makeNil();
        }

        Value error(const Vec<Value>& args) {
            for (size_t i = 0; i < args.size(); ++i) {
                if (i > 0) std::cerr << " ";
                std::cerr << args[i].toString();
            }
            std::cerr << std::endl;

            return Value::makeNil();
        }

    } 
} 