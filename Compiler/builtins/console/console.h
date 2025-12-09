#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace Console {
		Value print(const Vec<Value>& args);
		Value write(const Vec<Value>& args);
		Value error(const Vec<Value>& args);

	}
}