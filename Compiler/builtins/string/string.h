#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace String {
		Value length(const Vec<Value>& args);
		Value substring(const Vec<Value>& args);
		Value toupper(const Vec<Value>& args);
		Value tolower(const Vec<Value>& args);
		Value contains(const Vec<Value>& args);
		Value replace(const Vec<Value>& args);
		Value split(const Vec<Value>& args);
		Value trim(const Vec<Value>& args);
	} 
} 