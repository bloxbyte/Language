#include "Interpreter.h"
#include "../builtins/Builtins.h"
#include "../utils/Error.h"
#include <iostream>

Interpreter::Interpreter() : recursionDepth(0) {
    globalEnv = MAKE_PTR(Environment, nullptr);
    currentEnv = globalEnv;
}

void Interpreter::execute(Ptr<ProgramNode> program) {
    for (auto& def : program->definitions) {
        if (def->nodeType == ASTNodeType::FUNC_DEFINITION) {
            auto funcDef = std::static_pointer_cast<FuncDefinitionNode>(def);
            globalEnv->defineFunction(funcDef->name, funcDef);
        }
        else if (def->nodeType == ASTNodeType::STRUCT_DEFINITION) {
            auto structDef = std::static_pointer_cast<StructDefinitionNode>(def);
            globalEnv->defineStruct(structDef->name, structDef);
        }
    }

    for (auto& def : program->definitions) {
        if (def->nodeType == ASTNodeType::VAR_DEFINITION) {
            executeDefinition(def);
        }
    }
    if (globalEnv->hasFunction("Main")) {
        try {
            callFunction("Main", {});
        }
        catch (const ReturnException& e) {}
    }
}

void Interpreter::executeIfStatement(Ptr<IfNode> node) {
    Value condition = evaluate(node->condition);

    if (condition.isTruthy()) {
        for (auto& stmt : node->thenBranch) {
            executeStatement(stmt);
        }
        return;
    }

    for (auto& elseIfBranch : node->elseIfBranches) {
        Value elseIfCondition = evaluate(elseIfBranch.condition);
        if (elseIfCondition.isTruthy()) {
            for (auto& stmt : elseIfBranch.body) {
                executeStatement(stmt);
            }
            return;
        }
    }

    if (!node->elseBranch.empty()) {
        for (auto& stmt : node->elseBranch) {
            executeStatement(stmt);
        }
    }
}

void Interpreter::executeForStatement(Ptr<ForNode> node) {
    Value startVal = evaluate(node->start);
    Value endVal = evaluate(node->end);

    if (!startVal.isInt() || !endVal.isInt()) {
        throw TypeError("For loop range must be integers");
    }

    int start = startVal.asInt();
    int end = endVal.asInt();

    if (start > end) {
        return; 
    }

    currentEnv->define(node->iterator, Value::makeInt(start));
    Value iteratorValue = Value::makeInt(start);
    size_t bodySize = node->body.size();

    for (int i = start; i <= end; ++i) {
        iteratorValue.intValue = i;
        currentEnv->set(node->iterator, iteratorValue);

        for (size_t j = 0; j < bodySize; ++j) {
            executeStatement(node->body[j]);
        }
    }
}

void Interpreter::executeDefinition(Ptr<ASTNode> node) {
    switch (node->nodeType) {
    case ASTNodeType::VAR_DEFINITION:
        executeVarDefinition(std::static_pointer_cast<VarDefinitionNode>(node));
        break;
    case ASTNodeType::STRUCT_DEFINITION:
        executeStructDefinition(std::static_pointer_cast<StructDefinitionNode>(node));
        break;
    case ASTNodeType::FUNC_DEFINITION:
        executeFuncDefinition(std::static_pointer_cast<FuncDefinitionNode>(node));
        break;
    default:
        throw RuntimeError("Unknown definition type");
    }
}

void Interpreter::executeVarDefinition(Ptr<VarDefinitionNode> node) {
    if (node->names.size() != node->values.size()) {
        throw RuntimeError("Mismatch between number of names and values in definition", node->line);
    }

    for (size_t i = 0; i < node->names.size(); ++i) {
        Value value = evaluate(node->values[i]);
        currentEnv->define(node->names[i], value);
    }
}

void Interpreter::executeStructDefinition(Ptr<StructDefinitionNode> node) {}
void Interpreter::executeFuncDefinition(Ptr<FuncDefinitionNode> node) {}

void Interpreter::executeStatement(Ptr<ASTNode> node) {
    switch (node->nodeType) {
    case ASTNodeType::VAR_DEFINITION:
        executeVarDefinition(std::static_pointer_cast<VarDefinitionNode>(node));
        break;
    case ASTNodeType::ASSIGNMENT:
        executeAssignment(std::static_pointer_cast<AssignmentNode>(node));
        break;
    case ASTNodeType::IF_STMT:
        executeIfStatement(std::static_pointer_cast<IfNode>(node));
        break;
    case ASTNodeType::FOR_STMT:
        executeForStatement(std::static_pointer_cast<ForNode>(node));
        break;
    case ASTNodeType::RETURN_STMT:
        throw ReturnException(executeReturn(std::static_pointer_cast<ReturnNode>(node)));
    case ASTNodeType::CALL_EXPR:
        evaluate(node); 
        break;
    default:
        evaluate(node); 
        break;
    }
}

void Interpreter::executeAssignment(Ptr<AssignmentNode> node) {
    Value value = evaluate(node->value);
    currentEnv->set(node->identifier, value);
}

Value Interpreter::executeReturn(Ptr<ReturnNode> node) {
    if (node->value) {
        return evaluate(node->value);
    }
    return Value::makeNil();
}

Value Interpreter::evaluate(Ptr<ASTNode> node) {
    if (!node) {
        return Value::makeNil();
    }

    switch (node->nodeType) {
    case ASTNodeType::BINARY_EXPR:
        return evaluateBinary(std::static_pointer_cast<BinaryExprNode>(node));
    case ASTNodeType::UNARY_EXPR:
        return evaluateUnary(std::static_pointer_cast<UnaryExprNode>(node));
    case ASTNodeType::TERNARY_EXPR:
        return evaluateTernary(std::static_pointer_cast<TernaryExprNode>(node));
    case ASTNodeType::CALL_EXPR:
        return evaluateCall(std::static_pointer_cast<CallExprNode>(node));
    case ASTNodeType::LITERAL:
        return evaluateLiteral(std::static_pointer_cast<LiteralNode>(node));
    case ASTNodeType::IDENTIFIER:
        return evaluateIdentifier(std::static_pointer_cast<IdentifierNode>(node));
    case ASTNodeType::MEMBER_ACCESS:
        return evaluateMemberAccess(std::static_pointer_cast<MemberAccessNode>(node));
    default:
        throw RuntimeError("Cannot evaluate node type: " + std::to_string(static_cast<int>(node->nodeType)));
    }
}

Value Interpreter::evaluateBinary(Ptr<BinaryExprNode> node) {
    Value left = evaluate(node->left);
    Value right = evaluate(node->right);

    const String& op = node->op;

    if (left.isInt() && right.isInt()) {
        int l = left.asInt();
        int r = right.asInt();

        switch (op[0]) {
        case '+': return Value::makeInt(l + r);
        case '-': return Value::makeInt(l - r);
        case '*': return Value::makeInt(l * r);
        case '/':
            if (r == 0) throw RuntimeError("Division by zero");
            return Value::makeInt(l / r);
        case '%':
            if (r == 0) throw RuntimeError("Modulo by zero");
            return Value::makeInt(l % r);
        case '<':
            if (op.length() == 1) return Value::makeBool(l < r);
            return Value::makeBool(l <= r); 
        case '>':
            if (op.length() == 1) return Value::makeBool(l > r);
            return Value::makeBool(l >= r);
        case '=': 
            return Value::makeBool(l == r);
        case '!': 
            return Value::makeBool(l != r);
        }
    }
}

Value Interpreter::evaluateUnary(Ptr<UnaryExprNode> node) {
    Value operand = evaluate(node->operand);

    if (node->op == "-") {
        if (operand.isInt()) {
            return Value::makeInt(-operand.asInt());
        }
        throw TypeError("Unary '-' requires integer operand");
    }

    throw RuntimeError("Unknown unary operator: " + node->op);
}

Value Interpreter::evaluateTernary(Ptr<TernaryExprNode> node) {
    Value condition = evaluate(node->condition);

    if (condition.isTruthy()) {
        return evaluate(node->trueExpr);
    }
    else {
        return evaluate(node->falseExpr);
    }
}

Value Interpreter::evaluateCall(Ptr<CallExprNode> node) {
    Vec<Value> args;
    for (auto& argExpr : node->arguments) {
        args.push_back(evaluate(argExpr));
    }

    return callFunction(node->callee, args);
}

Value Interpreter::evaluateLiteral(Ptr<LiteralNode> node) {
    switch (node->litType) {
    case LiteralNode::LiteralType::INTEGER:
        return Value::makeInt(std::stoi(node->value));
    case LiteralNode::LiteralType::STRING:
        return Value::makeString(node->value);
    case LiteralNode::LiteralType::BOOLEAN:
        return Value::makeBool(node->value == "true");
    case LiteralNode::LiteralType::FLOAT:
        return Value::makeFloat(std::stof(node->value));
    default:
        throw RuntimeError("Unknown literal type");
    }
}

Value Interpreter::evaluateIdentifier(Ptr<IdentifierNode> node) {
    return currentEnv->get(node->name);
}

Value Interpreter::evaluateMemberAccess(Ptr<MemberAccessNode> node) {
    throw RuntimeError("Member access not yet implemented for non-function contexts");
}

Value Interpreter::callFunction(const String& name, const Vec<Value>& args) {
    auto builtinIt = builtinCache.find(name);

    if (builtinIt != builtinCache.end()) {
        if (builtinIt->second) {
            return callBuiltinFunction(name, args);
        }
    }
    else {
        bool isBuiltin = isBuiltinFunction(name);
        builtinCache[name] = isBuiltin;

        if (isBuiltin) {
            return callBuiltinFunction(name, args);
        }
    }

    auto funcIt = functionCache.find(name);
    if (funcIt != functionCache.end()) {
        return callUserFunction(funcIt->second, args);
    }

    if (currentEnv->hasFunction(name)) {
        auto func = currentEnv->getFunction(name);
        functionCache[name] = func;
        return callUserFunction(func, args);
    }

    throw NameError("Undefined function: " + name);
}

Value Interpreter::callUserFunction(Ptr<FuncDefinitionNode> func, const Vec<Value>& args) {
    checkRecursionDepth();

    if (args.size() != func->parameters.size()) {
        throw RuntimeError("Function '" + func->name + "' expects " +
            std::to_string(func->parameters.size()) + " arguments, got " +
            std::to_string(args.size()));
    }

    auto funcEnv = MAKE_PTR(Environment, currentEnv);

    for (size_t i = 0; i < func->parameters.size(); ++i) {
        funcEnv->define(func->parameters[i].name, args[i]);
    }

    auto prevEnv = currentEnv;
    currentEnv = funcEnv;
    recursionDepth++;

    try {
        for (auto& stmt : func->body) {
            executeStatement(stmt);
        }

        currentEnv = prevEnv;
        recursionDepth--;

        return Value::makeNil();

    }
    catch (const ReturnException& e) {
        currentEnv = prevEnv;
        recursionDepth--;
        return e.value;
    }
}

Value Interpreter::callBuiltinFunction(const String& name, const Vec<Value>& args) {
    return BuiltinRegistry::instance().callFunction(name, args);
}

bool Interpreter::isBuiltinFunction(const String& name) const {
    return BuiltinRegistry::instance().hasFunction(name);
}

void Interpreter::checkRecursionDepth() {
    if (recursionDepth >= Constants::MAX_RECURSION_DEPTH) {
        throw RuntimeError("Maximum recursion depth exceeded");
    }
}