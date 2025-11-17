#include <iomanip>
#include "tvShow.h"

namespace seneca
{
    TvShow::TvShow(const std::string& id,
                   const std::string& title,
                   unsigned short year,
                   const std::string& summary)
        : MediaItem(title, summary, year)
        , m_id{id}
    {
    }

    void TvShow::display(std::ostream& out) const
    {
        if (g_settings.m_tableView)
        {
            out << "S | ";
            out << std::left << std::setfill('.');
            out << std::setw(50) << this->getTitle() << " | ";
            out << std::right << std::setfill(' ');
            out << std::setw(2) << this->m_episodes.size() << " | ";
            out << std::setw(4) << this->getYear() << " | ";
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
            for (auto& item : m_episodes)
            {
                out << std::setfill('0') << std::right;
                out << "    " << 'S' << std::setw(2) << item.m_season
                    << 'E' << std::setw(2) << item.m_numberInSeason << ' ';
                if (!item.m_title.empty())
                    out << item.m_title << '\n';
                else
                    out << "Episode " << item.m_numberOverall << '\n';

                pos = 0;
                while (pos < item.m_summary.size())
                {
                    out << "            "
                        << item.m_summary.substr(pos, g_settings.m_maxSummaryWidth - 8) << '\n';
                    pos += g_settings.m_maxSummaryWidth - 8;
                }
            }
            out << std::setw(this->getTitle().size() + 7) << std::setfill('-') << ""
                << std::setfill(' ') << '\n';
        }
    }

    TvShow* TvShow::createItem(const std::string& strShow)
    {
        std::string line = strShow;
        MediaItem::trim(line);

        if (line.empty() || line[0] == '#')
            throw "Not a valid show.";

        std::stringstream ss(line);
        std::string id, title, yearStr, summary;

        if (!std::getline(ss, id, ',')) throw "Not a valid show.";
        if (!std::getline(ss, title, ',')) throw "Not a valid show.";
        if (!std::getline(ss, yearStr, ',')) throw "Not a valid show.";
        std::getline(ss, summary);

        MediaItem::trim(id);
        MediaItem::trim(title);
        MediaItem::trim(yearStr);
        MediaItem::trim(summary);

        if (id.empty() || title.empty() || yearStr.empty())
            throw "Not a valid show.";

        unsigned short year = static_cast<unsigned short>(std::stoul(yearStr));

        return new TvShow(id, title, year, summary);
    }

    double TvShow::getEpisodeAverageLength() const
    {
        if (m_episodes.empty())
            return 0.0;

        auto total = std::accumulate(
            m_episodes.begin(), m_episodes.end(), 0u,
            [](unsigned int acc, const TvEpisode& ep)
            {
                return acc + ep.m_length;
            });

        return static_cast<double>(total) / static_cast<double>(m_episodes.size());
    }

    std::list<std::string> TvShow::getLongEpisodes() const
    {
        // At least 1 hour = 3600 seconds
        const unsigned int minLen = 3600u;

        return std::accumulate(
            m_episodes.begin(), m_episodes.end(), std::list<std::string>{},
            [minLen](std::list<std::string> acc, const TvEpisode& ep)
            {
                if (ep.m_length >= minLen)
                {
                    if (!ep.m_title.empty())
                        acc.push_back(ep.m_title);
                    else
                        acc.push_back("Episode " + std::to_string(ep.m_numberOverall));
                }
                return acc;
            });
    }
}
