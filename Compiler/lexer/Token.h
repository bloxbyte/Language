#pragma once

#include "../Common.h"

enum class TokenType {
    INTEGER,
    STRING,
    TRUE,
    FALSE,

    IDENTIFIER,

    DEFINE,
    FLOAT,
    FLOAT_LITERAL,
    INT,
    STRING_TYPE,
    BOOL,
    STRUCT,
    FUNC,
    RETURN,

    IF,
    ELSEIF,     
    ELSE,
    FOR,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,

    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    EQUAL_EQUAL,
    NOT_EQUAL,

    AND,
    OR,

    COLON,          
    SEMICOLON,      
    COMMA,          
    DOT,            
    QUESTION,       
    LEFT_PAREN,     
    RIGHT_PAREN,    
    LEFT_BRACKET,   
    RIGHT_BRACKET,  
    LEFT_BRACE,     
    RIGHT_BRACE,    

    END_OF_FILE,
    INVALID
};

struct Token {
    TokenType type;
    String lexeme;
    int line;
    int column;

    Token(TokenType t, String lex, int ln, int col)
        : type(t), lexeme(lex), line(ln), column(col) {
    }

    String toString() const;
    bool is(TokenType t) const { return type == t; }
    bool isOneOf(TokenType t1, TokenType t2) const {
        return type == t1 || type == t2;
    }
    template<typename... Args>
    bool isOneOf(TokenType t1, TokenType t2, Args... rest) const {
        return type == t1 || isOneOf(t2, rest...);
    }
};

inline String Token::toString() const {
    return "Token(" + std::to_string(static_cast<int>(type)) +
        ", '" + lexeme + "', " +
        std::to_string(line) + ":" + std::to_string(column) + ")";
}