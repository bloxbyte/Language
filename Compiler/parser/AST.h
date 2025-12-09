#pragma once
#include "../Common.h"

class ASTVisitor;

enum class ASTNodeType {
    PROGRAM,
    VAR_DEFINITION,
    STRUCT_DEFINITION,
    FUNC_DEFINITION,
    ASSIGNMENT,
    RETURN_STMT,
    IF_STMT,        
    FOR_STMT,
    EXPR_STMT,
    BINARY_EXPR,
    UNARY_EXPR,
    TERNARY_EXPR,
    CALL_EXPR,
    LITERAL,
    IDENTIFIER,
    MEMBER_ACCESS
};

class ASTNode {
public:
    ASTNodeType nodeType;
    int line;
    ASTNode(ASTNodeType type, int ln = 0) : nodeType(type), line(ln) {}
    virtual ~ASTNode() = default;
};

class ForNode : public ASTNode {
public:
    String iterator;   
    Ptr<ASTNode> start;    
    Ptr<ASTNode> end;              
    Vec<Ptr<ASTNode>> body;        

    ForNode() : ASTNode(ASTNodeType::FOR_STMT) {}
};

class ProgramNode : public ASTNode {
public:
    Vec<Ptr<ASTNode>> definitions;
    ProgramNode() : ASTNode(ASTNodeType::PROGRAM) {}
};

class VarDefinitionNode : public ASTNode {
public:
    String type;
    Vec<String> names;
    Vec<Ptr<ASTNode>> values;
    VarDefinitionNode() : ASTNode(ASTNodeType::VAR_DEFINITION) {}
};

struct StructField {
    String type;
    String name;
};

class StructDefinitionNode : public ASTNode {
public:
    String name;
    Vec<StructField> fields;
    StructDefinitionNode() : ASTNode(ASTNodeType::STRUCT_DEFINITION) {}
};

struct Parameter {
    String type;
    String name;
};

class FuncDefinitionNode : public ASTNode {
public:
    String name;
    Vec<Parameter> parameters;
    Vec<Ptr<ASTNode>> body;
    FuncDefinitionNode() : ASTNode(ASTNodeType::FUNC_DEFINITION) {}
};

class AssignmentNode : public ASTNode {
public:
    String identifier;
    Ptr<ASTNode> value;
    AssignmentNode() : ASTNode(ASTNodeType::ASSIGNMENT) {}
};

class ReturnNode : public ASTNode {
public:
    Ptr<ASTNode> value;
    ReturnNode() : ASTNode(ASTNodeType::RETURN_STMT) {}
};

struct ElseIfBranch {
    Ptr<ASTNode> condition;
    Vec<Ptr<ASTNode>> body;
};

class IfNode : public ASTNode {
public:
    Ptr<ASTNode> condition;
    Vec<Ptr<ASTNode>> thenBranch;
    Vec<ElseIfBranch> elseIfBranches;
    Vec<Ptr<ASTNode>> elseBranch;

    IfNode() : ASTNode(ASTNodeType::IF_STMT) {}
};

class BinaryExprNode : public ASTNode {
public:
    String op;
    Ptr<ASTNode> left;
    Ptr<ASTNode> right;
    BinaryExprNode() : ASTNode(ASTNodeType::BINARY_EXPR) {}
};

class UnaryExprNode : public ASTNode {
public:
    String op;
    Ptr<ASTNode> operand;
    UnaryExprNode() : ASTNode(ASTNodeType::UNARY_EXPR) {}
};

class TernaryExprNode : public ASTNode {
public:
    Ptr<ASTNode> condition;
    Ptr<ASTNode> trueExpr;
    Ptr<ASTNode> falseExpr;
    TernaryExprNode() : ASTNode(ASTNodeType::TERNARY_EXPR) {}
};

class CallExprNode : public ASTNode {
public:
    String callee;
    Vec<Ptr<ASTNode>> arguments;
    CallExprNode() : ASTNode(ASTNodeType::CALL_EXPR) {}
};

class MemberAccessNode : public ASTNode {
public:
    String object;
    String member;
    MemberAccessNode() : ASTNode(ASTNodeType::MEMBER_ACCESS) {}
};

class LiteralNode : public ASTNode {
public:
    enum class LiteralType { INTEGER, STRING, BOOLEAN, FLOAT };
    LiteralType litType;
    String value;
    LiteralNode() : ASTNode(ASTNodeType::LITERAL) {}
};

class IdentifierNode : public ASTNode {
public:
    String name;
    IdentifierNode() : ASTNode(ASTNodeType::IDENTIFIER) {}
    explicit IdentifierNode(const String& n)
        : ASTNode(ASTNodeType::IDENTIFIER), name(n) {
    }
};