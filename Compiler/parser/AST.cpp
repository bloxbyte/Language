#include "AST.h"
#include <string>

namespace ASTUtil {
    std::string nodeTypeToString(ASTNodeType type) {
        switch (type) {
        case ASTNodeType::PROGRAM: return "PROGRAM";
        case ASTNodeType::VAR_DEFINITION: return "VAR_DEFINITION";
        case ASTNodeType::STRUCT_DEFINITION: return "STRUCT_DEFINITION";
        case ASTNodeType::FUNC_DEFINITION: return "FUNC_DEFINITION";
        case ASTNodeType::ASSIGNMENT: return "ASSIGNMENT";
        case ASTNodeType::RETURN_STMT: return "RETURN_STMT";
        case ASTNodeType::EXPR_STMT: return "EXPR_STMT";
        case ASTNodeType::BINARY_EXPR: return "BINARY_EXPR";
        case ASTNodeType::UNARY_EXPR: return "UNARY_EXPR";
        case ASTNodeType::TERNARY_EXPR: return "TERNARY_EXPR";
        case ASTNodeType::CALL_EXPR: return "CALL_EXPR";
        case ASTNodeType::LITERAL: return "LITERAL";
        case ASTNodeType::IDENTIFIER: return "IDENTIFIER";
        case ASTNodeType::MEMBER_ACCESS: return "MEMBER_ACCESS";
        default: return "UNKNOWN";
        }
    }
}