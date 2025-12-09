#include "Common.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "runtime/Interpreter.h"
#include "builtins/builtins.h"
#include "utils/Error.h"
#include <iostream>
#include <fstream>
#include <sstream>

String readFile(const String& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void runFile(const String& filename) {
    try {
        String source = readFile(filename);

        Lexer lexer(source);
        Vec<Token> tokens = lexer.tokenize();

        Parser parser(tokens);
        Ptr<ProgramNode> program = parser.parse();

        BuiltinRegistry::instance().registerAll();

        Interpreter interpreter;
        interpreter.execute(program);
    }

    catch (const CompilerError& e) {
        std::cerr << e.formatMessage() << std::endl;
        std::exit(1);
    }

    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::exit(1);
    }
}

void runREPL() {
    std::cout << "Language REPL v" << Constants::VERSION << std::endl;
    std::cout << "Type 'exit' to quit" << std::endl;

    BuiltinRegistry::instance().registerAll();

    Interpreter interpreter;
    String line;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line == "exit" || line == "quit") {
            break;
        }

        if (line.empty()) {
            continue;
        }

        try {
            Lexer lexer(line);
            Vec<Token> tokens = lexer.tokenize();

            Parser parser(tokens);
            Ptr<ProgramNode> program = parser.parse();

            interpreter.execute(program);

        }
        catch (const CompilerError& e) {
            std::cerr << e.formatMessage() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename.npp>" << std::endl;
        std::cout << "   or: " << argv[0] << " --repl" << std::endl;
        return 1;
    }

    String arg = argv[1];

    if (arg == "--repl" || arg == "-r") {
        runREPL();
    }
    else {
        runFile(arg);
    }

    return 0;
}