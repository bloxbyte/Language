#include "Parser.h"
#include "../utils/Error.h"

Parser::Parser(const Vec<Token>& toks) : tokens(toks), current(0) {}

Ptr<ProgramNode> Parser::parse() {
    auto program = MAKE_PTR(ProgramNode);

    while (!isAtEnd()) {
        try {
            program->definitions.push_back(parseDefinition());
        }
        catch (const ParserError& e) {
            while (!isAtEnd() && !check(TokenType::SEMICOLON)) {
                advance();
            }
            if (!isAtEnd()) advance(); 
            throw; 
        }
    }

    return program;
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(TokenType t1, TokenType t2) {
    if (check(t1) || check(t2)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const String& message) {
    if (check(type)) return advance();

    Token tok = peek();
    throw ParserError(message + ", got '" + tok.lexeme + "'", tok.line, tok.column);
}

Ptr<ASTNode> Parser::parseDefinition() {
    consume(TokenType::DEFINE, "Expected 'define'");

    Token typeToken = peek();

    if (match(TokenType::STRUCT)) {
        return parseStructDefinition();
    }
    else if (match(TokenType::FUNC)) {
        return parseFuncDefinition();
    }
    else if (match(TokenType::INT, TokenType::STRING_TYPE) || match(TokenType::BOOL) || match(TokenType::FLOAT)) {
        current--;
        return parseVarDefinition();
    }
    else {
        throw ParserError("Expected type, struct, or func after 'define'",
            typeToken.line, typeToken.column);
    }
}

Ptr<VarDefinitionNode> Parser::parseVarDefinition() {
    auto node = MAKE_PTR(VarDefinitionNode);
    node->line = peek().line;

    node->type = parseType();

    consume(TokenType::LEFT_BRACKET, "Expected '[' after type");
    node->names = parseNameList();
    consume(TokenType::RIGHT_BRACKET, "Expected ']' after name list");

    consume(TokenType::COLON, "Expected ':' after name list");

    consume(TokenType::LEFT_BRACKET, "Expected '[' before value list");
    node->values = parseValueList();
    consume(TokenType::RIGHT_BRACKET, "Expected ']' after value list");

    consume(TokenType::SEMICOLON, "Expected ';' after definition");

    return node;
}

Ptr<StructDefinitionNode> Parser::parseStructDefinition() {
    auto node = MAKE_PTR(StructDefinitionNode);
    node->line = peek().line;

    consume(TokenType::LEFT_BRACKET, "Expected '[' after 'struct'");
    Token nameToken = consume(TokenType::IDENTIFIER, "Expected struct name");
    node->name = nameToken.lexeme;
    consume(TokenType::RIGHT_BRACKET, "Expected ']' after struct name");

    consume(TokenType::COLON, "Expected ':' after struct name");

    consume(TokenType::LEFT_BRACKET, "Expected '[' before field list");
    node->fields = parseFieldList();
    consume(TokenType::RIGHT_BRACKET, "Expected ']' after field list");

    consume(TokenType::SEMICOLON, "Expected ';' after struct definition");

    return node;
}

Ptr<FuncDefinitionNode> Parser::parseFuncDefinition() {
    auto node = MAKE_PTR(FuncDefinitionNode);
    node->line = peek().line;

    consume(TokenType::LEFT_BRACKET, "Expected '[' after 'func'");
    Token nameToken = consume(TokenType::IDENTIFIER, "Expected function name");
    node->name = nameToken.lexeme;
    consume(TokenType::RIGHT_BRACKET, "Expected ']' after function name");

    consume(TokenType::COLON, "Expected ':' after function name");

    consume(TokenType::LEFT_BRACKET, "Expected '[' before parameter list");
    node->parameters = parseParameterList();
    consume(TokenType::RIGHT_BRACKET, "Expected ']' after parameter list");

    consume(TokenType::COMMA, "Expected ',' after parameter list");

    consume(TokenType::LEFT_BRACE, "Expected '{' to start function body");

    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        node->body.push_back(parseStatement());
    }

    consume(TokenType::RIGHT_BRACE, "Expected '}' to end function body");

    return node;
}

Ptr<ASTNode> Parser::parseStatement() {
    if (match(TokenType::DEFINE)) {
        current--; 
        return parseDefinition();
    }

    if (match(TokenType::IF)) {
        return parseIfStatement();
    }

    if (match(TokenType::FOR)) {
        return parseForStatement();
    }

    if (match(TokenType::RETURN)) {
        return parseReturn();
    }

    if (check(TokenType::IDENTIFIER)) {
        Token next = tokens[current + 1];
        if (next.type == TokenType::COLON) {
            return parseAssignment();
        }
    }

    auto expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression");
    return expr;
}

Ptr<ForNode> Parser::parseForStatement() {
    auto node = MAKE_PTR(ForNode);
    node->line = previous().line;

    Token iteratorToken = consume(TokenType::IDENTIFIER, "Expected iterator variable name after 'for'");
    node->iterator = iteratorToken.lexeme;

    consume(TokenType::COLON, "Expected ':' after iterator variable");

    consume(TokenType::LEFT_BRACKET, "Expected '[' before range");
    node->start = parseExpression();
    consume(TokenType::COMMA, "Expected ',' between range values");
    node->end = parseExpression();
    consume(TokenType::RIGHT_BRACKET, "Expected ']' after range");

    consume(TokenType::COMMA, "Expected ',' after range");

    consume(TokenType::LEFT_BRACE, "Expected '{' to start loop body");
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        node->body.push_back(parseStatement());
    }
    consume(TokenType::RIGHT_BRACE, "Expected '}' to end loop body");

    return node;
}

Ptr<AssignmentNode> Parser::parseAssignment() {
    auto node = MAKE_PTR(AssignmentNode);
    node->line = peek().line;

    Token name = consume(TokenType::IDENTIFIER, "Expected identifier");
    node->identifier = name.lexeme;

    consume(TokenType::COLON, "Expected ':' in assignment");

    node->value = parseExpression();

    consume(TokenType::SEMICOLON, "Expected ';' after assignment");

    return node;
}

Ptr<ReturnNode> Parser::parseReturn() {
    auto node = MAKE_PTR(ReturnNode);
    node->line = previous().line;

    if (!check(TokenType::SEMICOLON)) {
        node->value = parseExpression();
    }

    consume(TokenType::SEMICOLON, "Expected ';' after return");

    return node;
}

Ptr<ASTNode> Parser::parseExpression() {
    return parseTernary();
}

Ptr<ASTNode> Parser::parseTernary() {
    auto expr = parseLogicalOr();

    if (match(TokenType::QUESTION)) {
        auto node = MAKE_PTR(TernaryExprNode);
        node->condition = expr;
        node->trueExpr = parseExpression();
        consume(TokenType::COLON, "Expected ':' in ternary expression");
        node->falseExpr = parseExpression();
        return node;
    }

    return expr;
}

Ptr<ASTNode> Parser::parseLogicalOr() {
    auto expr = parseLogicalAnd();

    while (match(TokenType::OR)) {
        auto node = MAKE_PTR(BinaryExprNode);
        node->op = "||";
        node->left = expr;
        node->right = parseLogicalAnd();
        expr = node;
    }

    return expr;
}

Ptr<ASTNode> Parser::parseLogicalAnd() {
    auto expr = parseEquality();

    while (match(TokenType::AND)) {
        auto node = MAKE_PTR(BinaryExprNode);
        node->op = "&&";
        node->left = expr;
        node->right = parseEquality();
        expr = node;
    }

    return expr;
}

Ptr<ASTNode> Parser::parseEquality() {
    auto expr = parseComparison();

    while (match(TokenType::EQUAL_EQUAL, TokenType::NOT_EQUAL)) {
        auto node = MAKE_PTR(BinaryExprNode);
        node->op = previous().lexeme;
        node->left = expr;
        node->right = parseComparison();
        expr = node;
    }

    return expr;
}

Ptr<ASTNode> Parser::parseComparison() {
    auto expr = parseTerm();

    while (match(TokenType::LESS, TokenType::GREATER) ||
        match(TokenType::LESS_EQUAL, TokenType::GREATER_EQUAL)) {
        auto node = MAKE_PTR(BinaryExprNode);
        node->op = previous().lexeme;
        node->left = expr;
        node->right = parseTerm();
        expr = node;
    }

    return expr;
}

Ptr<ASTNode> Parser::parseTerm() {
    auto expr = parseFactor();

    while (match(TokenType::PLUS, TokenType::MINUS)) {
        auto node = MAKE_PTR(BinaryExprNode);
        node->op = previous().lexeme;
        node->left = expr;
        node->right = parseFactor();
        expr = node;
    }

    return expr;
}

Ptr<ASTNode> Parser::parseFactor() {
    auto expr = parseUnary();

    while (match(TokenType::STAR, TokenType::SLASH) || match(TokenType::PERCENT)) {
        auto node = MAKE_PTR(BinaryExprNode);
        node->op = previous().lexeme;
        node->left = expr;
        node->right = parseUnary();
        expr = node;
    }

    return expr;
}

Ptr<ASTNode> Parser::parseUnary() {
    if (match(TokenType::MINUS)) {
        auto node = MAKE_PTR(UnaryExprNode);
        node->op = "-";
        node->operand = parseUnary();
        return node;
    }

    return parsePrimary();
}

Ptr<ASTNode> Parser::parsePrimary() {
    if (match(TokenType::INTEGER)) {
        auto node = MAKE_PTR(LiteralNode);
        node->litType = LiteralNode::LiteralType::INTEGER;
        node->value = previous().lexeme;
        return node;
    }

    if (match(TokenType::FLOAT_LITERAL)) {
        auto node = MAKE_PTR(LiteralNode);
        node->litType = LiteralNode::LiteralType::FLOAT;
        node->value = previous().lexeme;
        return node;
    }

    if (match(TokenType::STRING)) {
        auto node = MAKE_PTR(LiteralNode);
        node->litType = LiteralNode::LiteralType::STRING;
        node->value = previous().lexeme;
        return node;
    }

    if (match(TokenType::TRUE)) {
        auto node = MAKE_PTR(LiteralNode);
        node->litType = LiteralNode::LiteralType::BOOLEAN;
        node->value = "true";
        return node;
    }

    if (match(TokenType::FALSE)) {
        auto node = MAKE_PTR(LiteralNode);
        node->litType = LiteralNode::LiteralType::BOOLEAN;
        node->value = "false";
        return node;
    }

    if (match(TokenType::LEFT_PAREN)) {
        auto expr = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return expr;
    }

    if (check(TokenType::STRING_TYPE) || check(TokenType::INT) ||
        check(TokenType::BOOL) || check(TokenType::FLOAT) ||
        check(TokenType::STRUCT) || check(TokenType::FUNC)) {
        Token name = advance();

        if (match(TokenType::DOT)) {
            Token member = peek();

            if (check(TokenType::IDENTIFIER)) {
                member = advance();
            }
            else if (check(TokenType::INT) || check(TokenType::STRING_TYPE) ||
                check(TokenType::BOOL) || check(TokenType::FLOAT) ||
                check(TokenType::FUNC) || check(TokenType::STRUCT)) {
                member = advance();
            }
            else {
                throw ParserError("Expected member name after '.'", peek().line, peek().column);
            }

            String fullName = name.lexeme + "." + member.lexeme;

            if (match(TokenType::LEFT_PAREN)) {
                auto callNode = MAKE_PTR(CallExprNode);
                callNode->callee = fullName;

                if (!check(TokenType::RIGHT_PAREN)) {
                    do {
                        callNode->arguments.push_back(parseExpression());
                    } while (match(TokenType::COMMA));
                }

                consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments");
                return callNode;
            }

            auto memberNode = MAKE_PTR(MemberAccessNode);
            memberNode->object = name.lexeme;
            memberNode->member = member.lexeme;
            return memberNode;
        }

        throw ParserError("Unexpected keyword '" + name.lexeme + "' in expression",
            name.line, name.column);
    }

    if (match(TokenType::IDENTIFIER)) {
        Token name = previous();

        if (match(TokenType::DOT)) {
            Token member = peek();

            if (check(TokenType::IDENTIFIER)) {
                member = advance();
            }
            else if (check(TokenType::INT) || check(TokenType::STRING_TYPE) ||
                check(TokenType::BOOL) || check(TokenType::FLOAT) ||
                check(TokenType::FUNC) || check(TokenType::STRUCT)) {
                member = advance();
            }
            else {
                throw ParserError("Expected member name after '.'", peek().line, peek().column);
            }

            String fullName = name.lexeme + "." + member.lexeme;

            if (match(TokenType::LEFT_PAREN)) {
                auto callNode = MAKE_PTR(CallExprNode);
                callNode->callee = fullName;

                if (!check(TokenType::RIGHT_PAREN)) {
                    do {
                        callNode->arguments.push_back(parseExpression());
                    } while (match(TokenType::COMMA));
                }

                consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments");
                return callNode;
            }

            auto memberNode = MAKE_PTR(MemberAccessNode);
            memberNode->object = name.lexeme;
            memberNode->member = member.lexeme;
            return memberNode;
        }

        if (match(TokenType::LEFT_PAREN)) {
            auto callNode = MAKE_PTR(CallExprNode);
            callNode->callee = name.lexeme;

            if (!check(TokenType::RIGHT_PAREN)) {
                do {
                    callNode->arguments.push_back(parseExpression());
                } while (match(TokenType::COMMA));
            }

            consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments");
            return callNode;
        }

        return MAKE_PTR(IdentifierNode, name.lexeme);
    }

    Token tok = peek();
    throw ParserError("Expected expression", tok.line, tok.column);
}


String Parser::parseType() {
    if (match(TokenType::INT)) return "int";
    if (match(TokenType::FLOAT)) return "float";
    if (match(TokenType::STRING_TYPE)) return "string";
    if (match(TokenType::BOOL)) return "bool";
    if (match(TokenType::IDENTIFIER)) return previous().lexeme; 

    Token tok = peek();
    throw ParserError("Expected type", tok.line, tok.column);
}

Vec<String> Parser::parseNameList() {
    Vec<String> names;

    do {
        Token name = consume(TokenType::IDENTIFIER, "Expected identifier in name list");
        names.push_back(name.lexeme);
    } while (match(TokenType::COMMA));

    return names;
}

Vec<Ptr<ASTNode>> Parser::parseValueList() {
    Vec<Ptr<ASTNode>> values;

    do {
        values.push_back(parseExpression());
    } while (match(TokenType::COMMA));

    return values;
}

Ptr<IfNode> Parser::parseIfStatement() {
    auto node = MAKE_PTR(IfNode);
    node->line = previous().line;

    consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'");
    node->condition = parseExpression();
    consume(TokenType::RIGHT_PAREN, "Expected ')' after condition");

    consume(TokenType::LEFT_BRACE, "Expected '{' after if condition");
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        node->thenBranch.push_back(parseStatement());
    }
    consume(TokenType::RIGHT_BRACE, "Expected '}' after if body");

    while (match(TokenType::ELSEIF)) {
        ElseIfBranch branch;

        consume(TokenType::LEFT_PAREN, "Expected '(' after 'elseif'");
        branch.condition = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after elseif condition");

        consume(TokenType::LEFT_BRACE, "Expected '{' after elseif condition");
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            branch.body.push_back(parseStatement());
        }
        consume(TokenType::RIGHT_BRACE, "Expected '}' after elseif body");

        node->elseIfBranches.push_back(branch);
    }

    if (match(TokenType::ELSE)) {
        consume(TokenType::LEFT_BRACE, "Expected '{' after else");
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            node->elseBranch.push_back(parseStatement());
        }
        consume(TokenType::RIGHT_BRACE, "Expected '}' after else body");
    }

    return node;
}

Vec<Parameter> Parser::parseParameterList() {
    Vec<Parameter> params;

    if (check(TokenType::RIGHT_BRACKET)) {
        return params;
    }

    do {
        Parameter param;
        param.type = parseType();
        Token name = consume(TokenType::IDENTIFIER, "Expected parameter name");
        param.name = name.lexeme;
        params.push_back(param);
    } while (match(TokenType::COMMA));

    return params;
}

Vec<StructField> Parser::parseFieldList() {
    Vec<StructField> fields;

    do {
        StructField field;
        field.type = parseType();
        Token name = consume(TokenType::IDENTIFIER, "Expected field name");
        field.name = name.lexeme;
        fields.push_back(field);
    } while (match(TokenType::COMMA));

    return fields;
}