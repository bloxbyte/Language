#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace System {
		Value exit(const Vec<Value>& args);
		Value pause(const Vec<Value>& args);
		Value version(const Vec<Value>& agrs);
	} 
} 