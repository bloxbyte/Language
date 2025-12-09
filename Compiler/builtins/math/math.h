#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace Math {

		// math.abs(x) - absolute value
		Value abs(const Vec<Value>& args);

		// math.min(a, b) - minimum of two values
		Value min(const Vec<Value>& args);

		// math.max(a, b) - maximum of two values
		Value max(const Vec<Value>& args);

		// math.pow(base, exp) - power function
		Value pow(const Vec<Value>& args);

		// math.sqrt(x) - square root
		Value sqrt(const Vec<Value>& args);

		// math.floor(x) - floor function
		Value floor(const Vec<Value>& args);

		// math.ceil(x) - ceiling function
		Value ceil(const Vec<Value>& args);

	} // namespace Math
} // namespace Builtins