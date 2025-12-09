#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace File {
		Value read(const Vec<Value>& args);
		Value write(const Vec<Value>& args);
		Value create(const Vec<Value>& args);
		Value exists(const Vec<Value>& args);
	} 
}