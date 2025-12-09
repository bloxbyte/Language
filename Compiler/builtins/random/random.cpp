#include "Random.h"
#include "../../utils/Error.h"
#include <random>

namespace Builtins {
    namespace Random {

        static std::random_device rd;
        static std::mt19937 gen(rd());

        Value randomInt(const Vec<Value>& args) {
            if (args.size() != 2) {
                throw RuntimeError("random.int() expects 2 arguments (min, max)");
            }

            if (!args[0].isInt() || !args[1].isInt()) {
                throw TypeError("random.int() requires integer arguments");
            }

            int min = args[0].asInt();
            int max = args[1].asInt();

            if (min > max) {
                std::swap(min, max);
            }

            std::uniform_int_distribution<int> dist(min, max);
            return Value::makeInt(dist(gen));
        }

        Value randomFloat(const Vec<Value>& args) {
            if (args.size() != 2) {
                throw RuntimeError("random.float() expects 2 arguments (min, max)");
            }

            double min = 0.0, max = 0.0;

            if (args[0].isFloat()) {
                min = static_cast<double>(args[0].asFloat());  
            }
            else {
                throw TypeError("random.float() requires decimal arguments");
            }

            if (args[1].isFloat()) {
                max = static_cast<double>(args[1].asFloat());  
            }
            else {
                throw TypeError("random.float() requires decimal arguments");
            }

            if (min > max) {
                std::swap(min, max);
            }

            std::uniform_real_distribution<double> dist(min, max);

            return Value::makeFloat(static_cast<float>(dist(gen))); 
        }

    } 
} 