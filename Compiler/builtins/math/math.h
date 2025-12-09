#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace Math {
		Value abs(const Vec<Value>& args);
		Value min(const Vec<Value>& args);
		Value max(const Vec<Value>& args);
		Value pow(const Vec<Value>& args);
		Value sqrt(const Vec<Value>& args);
		Value floor(const Vec<Value>& args);
		Value ceil(const Vec<Value>& args);
	} 
} 