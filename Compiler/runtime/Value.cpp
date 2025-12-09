#include "Value.h"

String Value::toString() const {
    switch (type) {
    case ValueType::INTEGER:
        return std::to_string(intValue);
    case ValueType::STRING:
        return stringValue;
    case ValueType::FLOAT:
        return std::to_string(floatValue);
    case ValueType::BOOLEAN:
        return boolValue ? "true" : "false";
    case ValueType::NIL:
        return "nil";
    case ValueType::FUNCTION:
        return "<function " + functionName + ">";
    case ValueType::STRUCT_INSTANCE:
        return "<struct " + structTypeName + ">";
    default:
        return "<unknown>";
    }
}

String Value::getTypeName() const {
    switch (type) {
    case ValueType::INTEGER: return "int";
    case ValueType::STRING: return "string";
    case ValueType::BOOLEAN: return "bool";
    case ValueType::FLOAT: return "float";
    case ValueType::NIL: return "nil";
    case ValueType::FUNCTION: return "function";
    case ValueType::STRUCT_INSTANCE: return "struct";
    default: return "unknown";
    }
}