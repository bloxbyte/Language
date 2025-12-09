#pragma once

#include "../Common.h"
#include "Token.h"

class Lexer {
public:
    explicit Lexer(const String& source);
    Vec<Token> tokenize();

private:
    String source;
    size_t current;
    int line;
    int column;
    Vec<Token> tokens;
    Map<String, TokenType> keywords;

    void initKeywords();
    void scanToken();
    void skipWhitespace();
    void skipLineComment();
    void skipMultilineComment();

    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);

    void addToken(TokenType type);
    void addToken(TokenType type, const String& lexeme);

    void scanString();
    void scanNumber();
    void scanIdentifier();

    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
};