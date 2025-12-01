/* Citation and Sources.... 
Assignment 04
Module: Utilities
FileName: Utilities.cpp
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 30, 2025
------------------------------------------------------------------
- Line 32-39 was assisted with the help of ChatGPT.
- Line 42-68 was assisted with the help of ChatGPT.
------------------------------------------------------------------*/
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "Utilities.h"

namespace seneca {
    char Utilities::m_delimiter = ',';

    void Utilities::setFieldWidth(size_t newWidth) {
        m_widthField = newWidth;
    }

    size_t Utilities::getFieldWidth() const {
        return m_widthField;
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    static std::string trim(const std::string& str) {
        const auto start = str.find_first_not_of(' ');
        if (start == std::string::npos) {
            return "";
        }
        const auto end = str.find_last_not_of(' ');
        return str.substr(start, end - start + 1);
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
        std::string token;
        size_t end_pos = str.find(m_delimiter, next_pos);

        if (next_pos == end_pos) {
            more = false;
            throw "Delimiter found at current position.";
        }

        if (end_pos == std::string::npos) {
            token = str.substr(next_pos);
            more = false;
            next_pos = str.length();
        } else {
            token = str.substr(next_pos, end_pos - next_pos);
            next_pos = end_pos + 1;
            more = true;
        }

        token = trim(token);

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