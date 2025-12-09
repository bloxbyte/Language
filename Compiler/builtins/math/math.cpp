#include "Math.h"
#include "../../utils/Error.h"
#include <cmath>
#include <algorithm>

namespace Builtins {
    namespace Math {

        Value abs(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("math.abs() expects 1 argument");
            }

            if (!args[0].isInt()) {
                throw TypeError("math.abs() requires integer argument");
            }

            int value = args[0].asInt();
            return Value::makeInt(value < 0 ? -value : value);
        }

        Value min(const Vec<Value>& args) {
            if (args.size() != 2) {
                throw RuntimeError("math.min() expects 2 arguments");
            }

            if (!args[0].isInt() || !args[1].isInt()) {
                throw TypeError("math.min() requires integer arguments");
            }

            int a = args[0].asInt();
            int b = args[1].asInt();
            return Value::makeInt(a < b ? a : b);
        }

        Value max(const Vec<Value>& args) {
            if (args.size() != 2) {
                throw RuntimeError("math.max() expects 2 arguments");
            }

            if (!args[0].isInt() || !args[1].isInt()) {
                throw TypeError("math.max() requires integer arguments");
            }

            int a = args[0].asInt();
            int b = args[1].asInt();
            return Value::makeInt(a > b ? a : b);
        }

        Value pow(const Vec<Value>& args) {
            if (args.size() != 2) {
                throw RuntimeError("math.pow() expects 2 arguments (base, exponent)");
            }

            if (!args[0].isInt() || !args[1].isInt()) {
                throw TypeError("math.pow() requires integer arguments");
            }

            int base = args[0].asInt();
            int exp = args[1].asInt();

            double result = std::pow(static_cast<double>(base), static_cast<double>(exp));
            return Value::makeInt(static_cast<int>(result));
        }

        Value sqrt(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("math.sqrt() expects 1 argument");
            }

            if (!args[0].isInt()) {
                throw TypeError("math.sqrt() requires integer argument");
            }

            int value = args[0].asInt();
            if (value < 0) {
                throw RuntimeError("math.sqrt() requires non-negative argument");
            }

            double result = std::sqrt(static_cast<double>(value));
            return Value::makeInt(static_cast<int>(result));
        }

        Value floor(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("math.floor() expects 1 argument");
            }

            if (!args[0].isInt()) {
                throw TypeError("math.floor() requires integer argument");
            }
     
            return args[0];
        }

        Value ceil(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("math.ceil() expects 1 argument");
            }

            if (!args[0].isInt()) {
                throw TypeError("math.ceil() requires integer argument");
            }

            return args[0];
        }

    }
} 