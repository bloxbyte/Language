#pragma once

#include "../Common.h"
#include "../lexer/Token.h"
#include "AST.h"

class Parser {
public:
    explicit Parser(const Vec<Token>& tokens);
    Ptr<ProgramNode> parse();

private:
    Vec<Token> tokens;
    size_t current;
    String callee;

    Token peek() const;
    Token previous() const;
    Token advance();
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool match(TokenType t1, TokenType t2);
    Token consume(TokenType type, const String& message);

    Ptr<ASTNode> parseDefinition();
    Ptr<VarDefinitionNode> parseVarDefinition();
    Ptr<StructDefinitionNode> parseStructDefinition();
    Ptr<FuncDefinitionNode> parseFuncDefinition();

    Ptr<ForNode> parseForStatement();
    Ptr<ASTNode> parseStatement();
    Ptr<AssignmentNode> parseAssignment();
    Ptr<ReturnNode> parseReturn();

    Ptr<ASTNode> parseExpression();
    Ptr<ASTNode> parseTernary();
    Ptr<ASTNode> parseLogicalOr();
    Ptr<ASTNode> parseLogicalAnd();
    Ptr<ASTNode> parseEquality();
    Ptr<ASTNode> parseComparison();
    Ptr<ASTNode> parseTerm();
    Ptr<ASTNode> parseFactor();
    Ptr<ASTNode> parseUnary();
    Ptr<ASTNode> parsePrimary();
    Ptr<IfNode> parseIfStatement();

    String parseType();
    Vec<String> parseNameList();
    Vec<Ptr<ASTNode>> parseValueList();
    Vec<Parameter> parseParameterList();
    Vec<StructField> parseFieldList();
};