#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "Utilities.h"

namespace seneca {
    // Initialize the static member
    char Utilities::m_delimiter = ',';

    void Utilities::setFieldWidth(size_t newWidth) {
        m_widthField = newWidth;
    }

    size_t Utilities::getFieldWidth() const {
        return m_widthField;
    }

    // Helper to trim leading/trailing whitespace
    static std::string trim(const std::string& str) {
        // Find first non-space character
        const auto start = str.find_first_not_of(' ');
        // If the string is all spaces or empty
        if (start == std::string::npos) {
            return "";
        }
        // Find last non-space character
        const auto end = str.find_last_not_of(' ');
        // Return the substring
        return str.substr(start, end - start + 1);
    }

    std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
        std::string token;
        size_t end_pos = str.find(m_delimiter, next_pos);

        // Check if a delimiter is found at the current position (empty token)
        if (next_pos == end_pos) {
            more = false;
            throw "Delimiter found at current position.";
        }

        if (end_pos == std::string::npos) {
            // Last token on the line
            token = str.substr(next_pos);
            more = false;
            next_pos = str.length();
        } else {
            // Token found
            token = str.substr(next_pos, end_pos - next_pos);
            next_pos = end_pos + 1; // Move past the delimiter
            more = true;
        }

        // Trim the token
        token = trim(token);

        // Update m_widthField if the extracted token is longer
        if (token.length() > m_widthField) {
            m_widthField = token.length();
        }

        return token;
    }

    void Utilities::setDelimiter(char newDelimiter) {
        m_delimiter = newDelimiter;
    }

    char Utilities::getDelimiter() {
        return m_delimiter;
    }
}