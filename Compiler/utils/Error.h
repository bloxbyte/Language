#pragma once

#include "../Common.h"
#include <exception>

class CompilerError : public std::runtime_error {
public:
    int line;
    int column;

    CompilerError(const String& msg, int ln = 0, int col = 0)
        : std::runtime_error(msg), line(ln), column(col) {}

    virtual String getType() const { return "CompilerError"; }

    String formatMessage() const {
        String msg = getType() + ": " + what();
        if (line > 0) {
            msg += " (line " + std::to_string(line);
            if (column > 0) {
                msg += ", column " + std::to_string(column);
            }
            msg += ")";
        }
        return msg;
    }
};

class LexerError : public CompilerError {
public:
    LexerError(const String& msg, int ln = 0, int col = 0)
        : CompilerError(msg, ln, col) {
    }

    String getType() const override { return "LexerError"; }
};

class ParserError : public CompilerError {
public:
    ParserError(const String& msg, int ln = 0, int col = 0)
        : CompilerError(msg, ln, col) {
    }

    String getType() const override { return "ParserError"; }
};

class RuntimeError : public CompilerError {
public:
    RuntimeError(const String& msg, int ln = 0)
        : CompilerError(msg, ln, 0) {
    }

    String getType() const override { return "RuntimeError"; }
};

class TypeError : public RuntimeError {
public:
    TypeError(const String& msg, int ln = 0)
        : RuntimeError(msg, ln) {
    }

    String getType() const override { return "TypeError"; }
};

class NameError : public RuntimeError {
public:
    NameError(const String& msg, int ln = 0)
        : RuntimeError(msg, ln) {
    }

    String getType() const override { return "NameError"; }
};