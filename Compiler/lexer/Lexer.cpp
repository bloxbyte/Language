#include "Lexer.h"
#include "../utils/Error.h"
#include <cctype>

Lexer::Lexer(const String& src)
    : source(src), current(0), line(1), column(1) {
    initKeywords();
}

void Lexer::initKeywords() {
    keywords["define"] = TokenType::DEFINE;
    keywords["int"] = TokenType::INT;
    keywords["string"] = TokenType::STRING_TYPE;
    keywords["bool"] = TokenType::BOOL;
    keywords["struct"] = TokenType::STRUCT;
    keywords["float"] = TokenType::FLOAT;
    keywords["func"] = TokenType::FUNC;
    keywords["return"] = TokenType::RETURN;
    keywords["if"] = TokenType::IF;
    keywords["elseif"] = TokenType::ELSEIF; 
    keywords["for"] = TokenType::FOR;
    keywords["else"] = TokenType::ELSE;
    keywords["true"] = TokenType::TRUE;
    keywords["false"] = TokenType::FALSE;
}

Vec<Token> Lexer::tokenize() {
    while (!isAtEnd()) {
        scanToken();
    }
    tokens.emplace_back(TokenType::END_OF_FILE, "", line, column);
    return tokens;
}

void Lexer::scanToken() {
    skipWhitespace();
    if (isAtEnd()) return;

    char c = advance();

    switch (c) {
    case '+': addToken(TokenType::PLUS, "+"); break;
    case '-':
        if (peek() == '-' && peekNext() == '/') {
            advance(); advance(); // consume --
            skipMultilineComment();
        }
        else {
            addToken(TokenType::MINUS, "-");
        }
        break;
    case '*': addToken(TokenType::STAR, "*"); break;
    case '/':
        if (peek() == '/') {
            skipLineComment();
        }
        else {
            addToken(TokenType::SLASH, "/");
        }
        break;
    case '%': addToken(TokenType::PERCENT, "%"); break;
    case ':': addToken(TokenType::COLON, ":"); break;
    case ';': addToken(TokenType::SEMICOLON, ";"); break;
    case ',': addToken(TokenType::COMMA, ","); break;
    case '.': addToken(TokenType::DOT, "."); break;
    case '?': addToken(TokenType::QUESTION, "?"); break;
    case '(': addToken(TokenType::LEFT_PAREN, "("); break;
    case ')': addToken(TokenType::RIGHT_PAREN, ")"); break;
    case '[': addToken(TokenType::LEFT_BRACKET, "["); break;
    case ']': addToken(TokenType::RIGHT_BRACKET, "]"); break;
    case '{': addToken(TokenType::LEFT_BRACE, "{"); break;
    case '}': addToken(TokenType::RIGHT_BRACE, "}"); break;
    case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS,
            match('=') ? "<=" : "<");
        break;
    case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER,
            match('=') ? ">=" : ">");
        break;
    case '=':
        if (match('=')) {
            addToken(TokenType::EQUAL_EQUAL, "==");
        }
        else {
            throw LexerError("Unexpected '=' at line " +
                std::to_string(line), line, column);
        }
        break;
    case '!':
        if (match('=')) {
            addToken(TokenType::NOT_EQUAL, "!=");
        }
        else {
            throw LexerError("Unexpected '!' at line " +
                std::to_string(line), line, column);
        }
        break;
    case '&':
        if (match('&')) {
            addToken(TokenType::AND, "&&");
        }
        else {
            throw LexerError("Unexpected '&' at line " +
                std::to_string(line), line, column);
        }
        break;
    case '|':
        if (match('|')) {
            addToken(TokenType::OR, "||");
        }
        else {
            throw LexerError("Unexpected '|' at line " +
                std::to_string(line), line, column);
        }
        break;
    case '"':
        scanString();
        break;
    default:
        if (isDigit(c)) {
            current--; column--;
            scanNumber();
        }
        else if (isAlpha(c)) {
            current--; column--;
            scanIdentifier();
        }
        else {
            throw LexerError("Unexpected character '" + String(1, c) +
                "' at line " + std::to_string(line), line, column);
        }
        break;
    }
}

void Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        }
        else if (c == '\n') {
            advance();
            line++;
            column = 1;
        }
        else {
            break;
        }
    }
}

void Lexer::skipLineComment() {
    while (!isAtEnd() && peek() != '\n') {
        advance();
    }
}

void Lexer::skipMultilineComment() {
    // Looking for \--
    while (!isAtEnd()) {
        if (peek() == '\\' && peekNext() == '-') {
            advance(); advance();
            if (peek() == '-') {
                advance();
                return;
            }
        }
        if (peek() == '\n') {
            line++;
            column = 0;
        }
        advance();
    }
    throw LexerError("Unterminated multiline comment", line, column);
}

void Lexer::scanString() {
    int startLine = line;
    int startCol = column - 1;
    String value;

    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\n') {
            line++;
            column = 0;
        }
        if (peek() == '\\') {
            advance();
            if (isAtEnd()) break;
            char escaped = advance();
            switch (escaped) {
            case 'n': value += '\n'; break;
            case 't': value += '\t'; break;
            case '"': value += '"'; break;
            case '\\': value += '\\'; break;
            default: value += escaped; break;
            }
        }
        else {
            value += advance();
        }
    }

    if (isAtEnd()) {
        throw LexerError("Unterminated string", startLine, startCol);
    }

    advance(); // closing "
    tokens.emplace_back(TokenType::STRING, value, startLine, startCol);
}

void Lexer::scanNumber() {
    int startCol = column;
    String number;

    // Scan integer part
    while (!isAtEnd() && isDigit(peek())) {
        number += advance();
    }

    // Check for decimal point followed by digits
    if (!isAtEnd() && peek() == '.' && peekNext() != '\0' && isDigit(peekNext())) {
        // Consume the '.'
        number += advance();

        // Scan fractional part
        while (!isAtEnd() && isDigit(peek())) {
            number += advance();
        }

        // Create float literal token
        tokens.emplace_back(TokenType::FLOAT_LITERAL, number, line, startCol);
        return;
    }

    // Create integer token
    tokens.emplace_back(TokenType::INTEGER, number, line, startCol);
}

void Lexer::scanIdentifier() {
    int startCol = column;
    String text;

    while (!isAtEnd() && isAlphaNumeric(peek())) {
        text += advance();
    }

    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;

    tokens.emplace_back(type, text, line, startCol);
}

bool Lexer::isAtEnd() const {
    return current >= source.length();
}

char Lexer::advance() {
    column++;
    return source[current++];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source[current] != expected) return false;
    current++;
    column++;
    return true;
}

void Lexer::addToken(TokenType type) {
    tokens.emplace_back(type, "", line, column - 1);
}

void Lexer::addToken(TokenType type, const String& lexeme) {
    tokens.emplace_back(type, lexeme, line, column - static_cast<int>(lexeme.length()));
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}