#pragma once

#include "../Common.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>

namespace StringUtil {
    inline String trimLeft(const String& str) {
        size_t start = str.find_first_not_of(" \t\n\r\f\v");
        return (start == String::npos) ? "" : str.substr(start);
    }

    inline String trimRight(const String& str) {
        size_t end = str.find_last_not_of(" \t\n\r\f\v");
        return (end == String::npos) ? "" : str.substr(0, end + 1);
    }

    inline String trim(const String& str) {
        return trimLeft(trimRight(str));
    }

    inline String toUpper(const String& str) {
        String result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    inline String toLower(const String& str) {
        String result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    inline bool startsWith(const String& str, const String& prefix) {
        if (prefix.length() > str.length()) return false;
        return str.compare(0, prefix.length(), prefix) == 0;
    }

    inline bool endsWith(const String& str, const String& suffix) {
        if (suffix.length() > str.length()) return false;
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }

    inline bool contains(const String& str, const String& substr) {
        return str.find(substr) != String::npos;
    }

    inline String replaceAll(String str, const String& from, const String& to) {
        size_t pos = 0;
        while ((pos = str.find(from, pos)) != String::npos) {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
        return str;
    }

    inline Vec<String> split(const String& str, char delimiter) {
        Vec<String> tokens;
        String token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    inline Vec<String> split(const String& str, const String& delimiter) {
        Vec<String> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);

        while (end != String::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }

        tokens.push_back(str.substr(start));
        return tokens;
    }

    inline String join(const Vec<String>& strings, const String& delimiter) {
        if (strings.empty()) return "";

        String result = strings[0];
        for (size_t i = 1; i < strings.size(); ++i) {
            result += delimiter + strings[i];
        }
        return result;
    }

    inline bool isInteger(const String& str) {
        if (str.empty()) return false;

        size_t start = 0;
        if (str[0] == '-' || str[0] == '+') {
            if (str.length() == 1) return false;
            start = 1;
        }

        for (size_t i = start; i < str.length(); ++i) {
            if (!std::isdigit(str[i])) return false;
        }

        return true;
    }
    
    inline bool isNumber(const String& str) {
        if (str.empty()) return false;

        bool hasDecimal = false;
        size_t start = 0;

        if (str[0] == '-' || str[0] == '+') {
            if (str.length() == 1) return false;
            start = 1;
        }

        for (size_t i = start; i < str.length(); ++i) {
            if (str[i] == '.') {
                if (hasDecimal) return false;
                hasDecimal = true;
            }
            else if (!std::isdigit(str[i])) {
                return false;
            }
        }

        return true;
    }

    inline String escape(const String& str) {
        String result;
        for (char c : str) {
            switch (c) {
            case '\n': result += "\\n"; break;
            case '\t': result += "\\t"; break;
            case '\r': result += "\\r"; break;
            case '\\': result += "\\\\"; break;
            case '"': result += "\\\""; break;
            default: result += c; break;
            }
        }
        return result;
    }

    inline String unescape(const String& str) {
        String result;
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == '\\' && i + 1 < str.length()) {
                switch (str[i + 1]) {
                case 'n': result += '\n'; i++; break;
                case 't': result += '\t'; i++; break;
                case 'r': result += '\r'; i++; break;
                case '\\': result += '\\'; i++; break;
                case '"': result += '"'; i++; break;
                default: result += str[i]; break;
                }
            }
            else {
                result += str[i];
            }
        }
        return result;
    }

    inline String repeat(const String& str, size_t count) {
        String result;
        result.reserve(str.length() * count);
        for (size_t i = 0; i < count; ++i) {
            result += str;
        }
        return result;
    }

    inline String padLeft(const String& str, size_t width, char padChar = ' ') {
        if (str.length() >= width) return str;
        return String(width - str.length(), padChar) + str;
    }

    inline String padRight(const String& str, size_t width, char padChar = ' ') {
        if (str.length() >= width) return str;
        return str + String(width - str.length(), padChar);
    }

    inline String formatError(const String& message, int line, int column = -1) {
        String result = message;
        if (line > 0) {
            result += " at line " + std::to_string(line);
            if (column >= 0) {
                result += ", column " + std::to_string(column);
            }
        }
        return result;
    }

    inline String toString(int value) {
        return std::to_string(value);
    }

    inline String toString(bool value) {
        return value ? "true" : "false";
    }

    inline bool tryParseInt(const String& str, int& out) {
        try {
            size_t pos;
            out = std::stoi(str, &pos);
            return pos == str.length();
        }
        catch (...) {
            return false;
        }
    }

    inline String quote(const String& str) {
        return "\"" + str + "\"";
    }

    inline String truncate(const String& str, size_t maxLength, const String& ellipsis = "...") {
        if (str.length() <= maxLength) return str;
        if (maxLength <= ellipsis.length()) return ellipsis.substr(0, maxLength);
        return str.substr(0, maxLength - ellipsis.length()) + ellipsis;
    }
} 