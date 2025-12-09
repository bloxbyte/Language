// Random.h
#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace Random {

		// random.int(min, max) - returns random integer between min and max (inclusive)
		Value randomInt(const Vec<Value>& args);

		// random.float(min, max) - returns random float between min and max
		Value randomFloat(const Vec<Value>& args);

	} // namespace Random
} // namespace Builtins