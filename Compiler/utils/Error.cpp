#include "Error.h"

#include <sstream>

namespace ErrorUtil {
    String formatLocation(int line, int column) {
        std::stringstream ss;
        ss << "line " << line;
        if (column > 0) {
            ss << ", column " << column;
        }
        return ss.str();
    }

    String formatError(const String& type, const String& message, int line, int column) {
        String result = type + ": " + message;
        if (line > 0) {
            result += " (" + formatLocation(line, column) + ")";
        }
        return result;
    }
}