#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace String {

		// string.length(s) - get string length
		Value length(const Vec<Value>& args);

		// string.substring(s, start, end) - get substring
		Value substring(const Vec<Value>& args);

		// string.toupper(s) - convert to uppercase
		Value toupper(const Vec<Value>& args);

		// string.tolower(s) - convert to lowercase
		Value tolower(const Vec<Value>& args);

		// string.contains(s, substr) - check if contains substring
		Value contains(const Vec<Value>& args);

		// string.replace(s, old, new) - replace all occurrences
		Value replace(const Vec<Value>& args);

		// string.split(s, delim) - split string (returns concatenated result for now)
		Value split(const Vec<Value>& args);

		// string.trim(s) - remove leading/trailing whitespace
		Value trim(const Vec<Value>& args);

	} // namespace String
} // namespace Builtins