#include "file.h"
#include "../../utils/Error.h"
#include <fstream>
#include <sstream>

namespace Builtins {
    namespace File {

        Value read(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("file.read() expects 1 argument (filename)");
            }

            if (!args[0].isString()) {
                throw TypeError("file.read() requires string filename");
            }

            String filename = args[0].asString();
            std::ifstream file(filename);

            if (!file.is_open()) {
                throw RuntimeError("Failed to open file: " + filename);
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();

            return Value::makeString(buffer.str());
        }

        Value write(const Vec<Value>& args) {
            if (args.size() != 2) {
                throw RuntimeError("file.write() expects 2 arguments (filename, content)");
            }

            if (!args[0].isString()) {
                throw TypeError("file.write() requires string filename");
            }

            if (!args[1].isString()) {
                throw TypeError("file.write() requires string content");
            }

            String filename = args[0].asString();
            String content = args[1].asString();

            std::ofstream file(filename);

            if (!file.is_open()) {
                throw RuntimeError("Failed to open file for writing: " + filename);
            }

            file << content;
            file.close();

            return Value::makeBool(true);
        }

        Value create(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("file.create() expects 1 argument (filename)");
            }

            if (!args[0].isString()) {
                throw TypeError("file.create() requires string filename");
            }

            String filename = args[0].asString();
            std::ofstream file(filename);

            if (!file.is_open()) {
                throw RuntimeError("Failed to create file: " + filename);
            }

            file.close();
            return Value::makeBool(true);
        }

        Value exists(const Vec<Value>& args) {
            if (args.size() != 1) {
                throw RuntimeError("file.exists() expects 1 argument (filename)");
            }

            if (!args[0].isString()) {
                throw TypeError("file.exists() requires string filename");
            }

            String filename = args[0].asString();
            std::ifstream file(filename);
            bool fileExists = file.good();
            file.close();

            return Value::makeBool(fileExists);
        }

    } 
} 