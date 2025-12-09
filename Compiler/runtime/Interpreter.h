#pragma once

#include "../Common.h"
#include "../parser/AST.h"
#include "Value.h"
#include "Environment.h"

class Interpreter {
public:
    Interpreter();
    void execute(Ptr<ProgramNode> program);

private:
    Ptr<Environment> globalEnv;
    Ptr<Environment> currentEnv;
    int recursionDepth;

    Map<String, Ptr<FuncDefinitionNode>> functionCache;
    Map<String, bool> builtinCache;

    void executeDefinition(Ptr<ASTNode> node);
    void executeVarDefinition(Ptr<VarDefinitionNode> node);
    void executeForStatement(Ptr<ForNode> node);
    void executeIfStatement(Ptr<IfNode> node);
    void executeStructDefinition(Ptr<StructDefinitionNode> node);
    void executeFuncDefinition(Ptr<FuncDefinitionNode> node);

    void executeStatement(Ptr<ASTNode> node);
    void executeAssignment(Ptr<AssignmentNode> node);
    Value executeReturn(Ptr<ReturnNode> node);

    Value evaluate(Ptr<ASTNode> node);
    Value evaluateBinary(Ptr<BinaryExprNode> node);
    Value evaluateUnary(Ptr<UnaryExprNode> node);
    Value evaluateTernary(Ptr<TernaryExprNode> node);
    Value evaluateCall(Ptr<CallExprNode> node);
    Value evaluateLiteral(Ptr<LiteralNode> node);
    Value evaluateIdentifier(Ptr<IdentifierNode> node);
    Value evaluateMemberAccess(Ptr<MemberAccessNode> node);

    Value callFunction(const String& name, const Vec<Value>& args);
    Value callUserFunction(Ptr<FuncDefinitionNode> func, const Vec<Value>& args);
    Value callBuiltinFunction(const String& name, const Vec<Value>& args);

    bool isBuiltinFunction(const String& name) const;
    void checkRecursionDepth();

    struct ReturnException {
        Value value;
        explicit ReturnException(const Value& v) : value(v) {}
    };
};