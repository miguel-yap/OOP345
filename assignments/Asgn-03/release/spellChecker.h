#ifndef SENECA_SPELLCHECKER_H
#define SENECA_SPELLCHECKER_H

#include <string>
#include <array>
#include <iostream>

namespace seneca
{
    class SpellChecker
    {
        std::array<std::string, 6> m_badWords{};
        std::array<std::string, 6> m_goodWords{};
        std::array<size_t, 6>      m_replacements{};

    public:
        SpellChecker(const char* filename);

        void operator()(std::string& text);

        void showStatistics(std::ostream& out) const;
    };
}

#endif
