#include "Utilities.h"
#include <stdexcept>
#include <cstddef>

namespace {

    // trim helpers in anonymous namespace
    inline std::string ltrim(const std::string& s) {
        size_t start = 0u;
        while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
            ++start;
        }
        return s.substr(start);
    }

    inline std::string rtrim(const std::string& s) {
        if (s.empty()) return s;
        size_t end = s.size();
        while (end > 0 && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
            --end;
        }
        return s.substr(0, end);
    }

    inline std::string trim(const std::string& s) {
        return rtrim(ltrim(s));
    }

} // anonymous namespace

namespace seneca {

    char Utilities::m_delimiter = '\0';

    void Utilities::setFieldWidth(size_t newWidth) {
        m_widthField = newWidth;
    }

    size_t Utilities::getFieldWidth() const {
        return m_widthField;
    }

    std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
        if (next_pos > str.size()) {
            more = false;
            return {};
        }

        if (next_pos == str.size()) {
            more = false;
            throw std::runtime_error("No token");
        }

        if (str[next_pos] == m_delimiter) {
            more = false;
            throw std::runtime_error("No token");
        }

        size_t end_pos = str.find(m_delimiter, next_pos);
        std::string token;

        if (end_pos == std::string::npos) {
            token = str.substr(next_pos);
            more = false;
            next_pos = str.size() + 1;
        } else {
            token = str.substr(next_pos, end_pos - next_pos);
            more = true;
            next_pos = end_pos + 1;
        }

        token = trim(token);

        if (token.empty()) {
            more = false;
            throw std::runtime_error("No token");
        }

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

} // namespace seneca
