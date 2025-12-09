#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace Random {
		Value randomInt(const Vec<Value>& args);
		Value randomFloat(const Vec<Value>& args);
	}
} 