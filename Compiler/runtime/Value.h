#pragma once
#include "../Common.h"

enum class ValueType {
    INTEGER,
    FLOAT,      
    STRING,
    BOOLEAN,
    NIL,
    FUNCTION,
    STRUCT_INSTANCE
};

class Value {
public:
    inline bool isInt() const { return type == ValueType::INTEGER; }
    inline bool isFloat() const { return type == ValueType::FLOAT; }
    inline bool isString() const { return type == ValueType::STRING; }
    inline bool isBool() const { return type == ValueType::BOOLEAN; }

    inline int asInt() const { return intValue; }
    inline float asFloat() const { return floatValue; }
    inline bool asBool() const { return boolValue; }

    ValueType type;

    int intValue;
    float floatValue;   
    String stringValue;
    bool boolValue;

    String functionName;

    Map<String, Value> structFields;
    String structTypeName;

    Value() : type(ValueType::NIL), intValue(0), floatValue(0.0f), boolValue(false) {}

    static Value makeInt(int val) {
        Value v;
        v.type = ValueType::INTEGER;
        v.intValue = val;
        return v;
    }

    static Value makeFloat(float val) {
        Value v;
        v.type = ValueType::FLOAT;
        v.floatValue = val;
        return v;
    }

    static Value makeString(const String& val) {
        Value v;
        v.type = ValueType::STRING;
        v.stringValue = val;
        return v;
    }

    static Value makeBool(bool val) {
        Value v;
        v.type = ValueType::BOOLEAN;
        v.boolValue = val;
        return v;
    }

    static Value makeNil() {
        return Value();
    }

    static Value makeFunction(const String& name) {
        Value v;
        v.type = ValueType::FUNCTION;
        v.functionName = name;
        return v;
    }

    bool isNil() const { return type == ValueType::NIL; }
    bool isFunction() const { return type == ValueType::FUNCTION; }
    bool isStruct() const { return type == ValueType::STRUCT_INSTANCE; }
            
    const String& asString() const { return stringValue; }

    String toString() const;
    String getTypeName() const;

    bool isTruthy() const {
        if (isBool()) return boolValue;
        if (isInt()) return intValue != 0;
        if (isFloat()) return floatValue != 0.0f;               
        if (isString()) return !stringValue.empty();
        return false;
    }
};