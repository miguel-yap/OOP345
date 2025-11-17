/* Citation and Sources.... 
Assignment 03
Module: movie
FileName: movie.cpp
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 16, 2025
------------------------------------------------------------------
- Line 63-88 was assisted with the help of ChatGPT.
------------------------------------------------------------------*/
#include <sstream>
#include <iomanip>
#include "movie.h"

namespace seneca
{
    Movie::Movie(const std::string& title,
                 unsigned short year,
                 const std::string& summary)
        : MediaItem(title, summary, year)
    {
    }

    void Movie::display(std::ostream& out) const
    {
        if (g_settings.m_tableView)
        {
            out << "M | ";
            out << std::left << std::setfill('.');
            out << std::setw(50) << this->getTitle() << " | ";
            out << std::right << std::setfill(' ');
            out << std::setw(9) << this->getYear() << " | ";
            out << std::left;
            if (g_settings.m_maxSummaryWidth > -1)
            {
                if (static_cast<short>(this->getSummary().size()) <= g_settings.m_maxSummaryWidth)
                    out << this->getSummary();
                else
                    out << this->getSummary().substr(0, g_settings.m_maxSummaryWidth - 3) << "...";
            }
            else
                out << this->getSummary();
            out << std::endl;
        }
        else
        {
            size_t pos = 0;
            out << this->getTitle() << " [" << this->getYear() << "]\n";
            out << std::setw(this->getTitle().size() + 7) << std::setfill('-') << "" << '\n';
            while (pos < this->getSummary().size())
            {
                out << "    " << this->getSummary().substr(pos, g_settings.m_maxSummaryWidth) << '\n';
                pos += g_settings.m_maxSummaryWidth;
            }
            out << std::setw(this->getTitle().size() + 7) << std::setfill('-') << ""
                << std::setfill(' ') << '\n';
        }
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    Movie* Movie::createItem(const std::string& strMovie)
    {
        std::string line = strMovie;
        MediaItem::trim(line);

        if (line.empty() || line[0] == '#')
            throw "Not a valid movie.";

        std::stringstream ss(line);
        std::string title, yearStr, summary;

        if (!std::getline(ss, title, ',')) throw "Not a valid movie.";
        if (!std::getline(ss, yearStr, ',')) throw "Not a valid movie.";
        std::getline(ss, summary);   

        MediaItem::trim(title);
        MediaItem::trim(yearStr);
        MediaItem::trim(summary);

        if (title.empty() || yearStr.empty())
            throw "Not a valid movie.";

        unsigned short year = static_cast<unsigned short>(std::stoul(yearStr));

        return new Movie(title, year, summary);
    }
}
