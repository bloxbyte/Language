#pragma once

#include "../../Common.h"
#include "../../runtime/Value.h"

namespace Builtins {
	namespace File {

		// file.read(filename) - read entire file as string
		Value read(const Vec<Value>& args);

		// file.write(filename, content) - write string to file
		Value write(const Vec<Value>& args);

		// file.create(filename) - create empty file
		Value create(const Vec<Value>& args);

		// file.exists(filename) - check if file exists
		Value exists(const Vec<Value>& args);

	} // namespace File
} // namespace Builtins