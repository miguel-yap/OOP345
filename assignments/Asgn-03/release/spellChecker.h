/* Citation and Sources.... 
Assignment 03
Module: spellChecker
FileName: spellChecker.h
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 16, 2025
------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. 
This submitted piece of work has not been shared with any other student or 3rd party content provider.
------------------------------------------------------------------*/
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
