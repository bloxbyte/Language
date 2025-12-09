#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace Console {

		// console.print(value) - prints value with newline
		Value print(const Vec<Value>& args);

		// console.write(value) - prints value without newline
		Value write(const Vec<Value>& args);

		// console.error(message) - prints to stderr
		Value error(const Vec<Value>& args);

	} // namespace Console
} // namespace Builtins