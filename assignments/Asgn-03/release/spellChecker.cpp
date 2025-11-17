#include <fstream>
#include <iomanip>
#include "spellChecker.h"

namespace seneca
{
    SpellChecker::SpellChecker(const char* filename)
    {
        std::ifstream file(filename);
        if (!file)
            throw "Bad file name!";

        for (size_t i = 0; i < m_badWords.size(); ++i)
        {
            if (!(file >> m_badWords[i] >> m_goodWords[i]))
                throw "Bad file name.";
            m_replacements[i] = 0u;
        }
    }

    void SpellChecker::operator()(std::string& text)
    {
        for (size_t i = 0; i < m_badWords.size(); ++i)
        {
            const std::string& bad = m_badWords[i];
            const std::string& good = m_goodWords[i];

            std::size_t pos = text.find(bad);
            while (pos != std::string::npos)
            {
                text.replace(pos, bad.length(), good);
                ++m_replacements[i];
                pos = text.find(bad, pos + good.length());
            }
        }
    }

    void SpellChecker::showStatistics(std::ostream& out) const
    {
        out << "Spellchecker Statistics\n";
        for (size_t i = 0; i < m_badWords.size(); ++i)
        {
            out << std::left << std::setw(15) << m_badWords[i]
                << " : " << m_replacements[i] << " replacements\n";
        }
    }
}
