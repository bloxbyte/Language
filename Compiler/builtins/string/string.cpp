#include "String.h"
#include "../../utils/Error.h"
#include <algorithm>
#include <cctype>

namespace Builtins {
    namespace String {

        Value length(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("string.length() expects 1 argument");
            }

            if (!args[0].isString()) {
                throw TypeError("string.length() requires string argument");
            }

            return Value::makeInt(static_cast<int>(args[0].asString().length()));
        }

        Value substring(const Vec<Value>& args) {
            if (args.size() != 3) {
                throw RuntimeError("string.substring() expects 3 arguments (string, start, end)");
            }

            if (!args[0].isString()) {
                throw TypeError("string.substring() requires string as first argument");
            }

            if (!args[1].isInt() || !args[2].isInt()) {
                throw TypeError("string.substring() requires integer indices");
            }

            ::String str = args[0].asString();
            int start = args[1].asInt();
            int end = args[2].asInt();

            if (start < 0) start = 0;
            if (end > static_cast<int>(str.length())) end = static_cast<int>(str.length());
            if (start >= end) return Value::makeString("");

            return Value::makeString(str.substr(start, end - start));
        }

        Value toupper(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("string.toupper() expects 1 argument");
            }

            if (!args[0].isString()) {
                throw TypeError("string.toupper() requires string argument");
            }

            ::String str = args[0].asString();
            std::transform(str.begin(), str.end(), str.begin(),
                [](unsigned char c) { return std::toupper(c); });

            return Value::makeString(str);
        }

        Value tolower(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("string.tolower() expects 1 argument");
            }

            if (!args[0].isString()) {
                throw TypeError("string.tolower() requires string argument");
            }

            ::String str = args[0].asString();
            std::transform(str.begin(), str.end(), str.begin(),
                [](unsigned char c) { return std::tolower(c); });

            return Value::makeString(str);
        }

        Value contains(const Vec<Value>& args) {
            if (args.size() != 2) {
                throw RuntimeError("string.contains() expects 2 arguments (string, substring)");
            }

            if (!args[0].isString() || !args[1].isString()) {
                throw TypeError("string.contains() requires string arguments");
            }

            ::String str = args[0].asString();
            ::String substr = args[1].asString();

            return Value::makeBool(str.find(substr) != ::String::npos);
        }

        Value replace(const Vec<Value>& args) {
            if (args.size() != 3) {
                throw RuntimeError("string.replace() expects 3 arguments (string, old, new)");
            }

            if (!args[0].isString() || !args[1].isString() || !args[2].isString()) {
                throw TypeError("string.replace() requires string arguments");
            }

            ::String str = args[0].asString();
            ::String oldStr = args[1].asString();
            ::String newStr = args[2].asString();

            size_t pos = 0;
            while ((pos = str.find(oldStr, pos)) != ::String::npos) {
                str.replace(pos, oldStr.length(), newStr);
                pos += newStr.length();
            }

            return Value::makeString(str);
        }

        Value split(const Vec<Value>& args) {
            if (args.size() != 2) {
                throw RuntimeError("string.split() expects 2 arguments (string, delimiter)");
            }

            if (!args[0].isString() || !args[1].isString()) {
                throw TypeError("string.split() requires string arguments");
            }

            ::String str = args[0].asString();
            ::String delim = args[1].asString();

            // Since we don't have arrays yet, return count of splits as int
            int count = 0;
            size_t pos = 0;
            while ((pos = str.find(delim, pos)) != ::String::npos) {
                count++;
                pos += delim.length();
            }

            return Value::makeInt(count + 1); 
        }

        Value trim(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("string.trim() expects 1 argument");
            }

            if (!args[0].isString()) {
                throw TypeError("string.trim() requires string argument");
            }

            ::String str = args[0].asString();
           
            size_t start = str.find_first_not_of(" \t\n\r");
            if (start == ::String::npos) {
                return Value::makeString("");
            }

            size_t end = str.find_last_not_of(" \t\n\r");

            return Value::makeString(str.substr(start, end - start + 1));
        }

    }
} 