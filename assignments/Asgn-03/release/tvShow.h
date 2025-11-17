/* Citation and Sources.... 
Assignment 03
Module: tvShow
FileName: tvShow.h
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 16, 2025
------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. 
This submitted piece of work has not been shared with any other student or 3rd party content provider.
------------------------------------------------------------------*/
#ifndef SENECA_TVSHOW_H
#define SENECA_TVSHOW_H

#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <algorithm>
#include <numeric>
#include "mediaItem.h"
#include "settings.h"

namespace seneca
{
    class TvShow;   

    struct TvEpisode
    {
        const TvShow* m_show{};
        unsigned short m_numberOverall{};
        unsigned short m_season{};
        unsigned short m_numberInSeason{};
        std::string m_airDate{};
        unsigned int m_length{};      
        std::string m_title{};
        std::string m_summary{};
    };

    class TvShow : public MediaItem
    {
        std::string m_id{};
        std::vector<TvEpisode> m_episodes{};

        TvShow(const std::string& id,
               const std::string& title,
               unsigned short year,
               const std::string& summary);

        static unsigned int parseLengthToSeconds(const std::string& lenStr)
        {
            std::stringstream ss(lenStr);
            std::string h, m, s;
            std::getline(ss, h, ':');
            std::getline(ss, m, ':');
            std::getline(ss, s, ':');
            unsigned int hh = h.empty() ? 0u : static_cast<unsigned int>(std::stoul(h));
            unsigned int mm = m.empty() ? 0u : static_cast<unsigned int>(std::stoul(m));
            unsigned int ssVal = s.empty() ? 0u : static_cast<unsigned int>(std::stoul(s));
            return hh * 3600u + mm * 60u + ssVal;
        }

    public:
        void display(std::ostream& out = std::cout) const override;

        static TvShow* createItem(const std::string& strShow);

        template<typename Collection_t>
        static void addEpisode(Collection_t& col, const std::string& strEpisode)
        {
            std::string line = strEpisode;
            MediaItem::trim(line);
            if (line.empty() || line[0] == '#')
                throw "Not a valid episode.";

            std::stringstream ss(line);
            std::string id, overallStr, seasonStr, inSeasonStr,
                        airDate, lengthStr, title, summary;

            if (!std::getline(ss, id, ',')) throw "Not a valid episode.";
            if (!std::getline(ss, overallStr, ',')) throw "Not a valid episode.";
            if (!std::getline(ss, seasonStr, ',')) throw "Not a valid episode.";
            if (!std::getline(ss, inSeasonStr, ',')) throw "Not a valid episode.";
            if (!std::getline(ss, airDate, ',')) throw "Not a valid episode.";
            if (!std::getline(ss, lengthStr, ',')) throw "Not a valid episode.";
            if (!std::getline(ss, title, ',')) throw "Not a valid episode.";
            std::getline(ss, summary);

            MediaItem::trim(id);
            MediaItem::trim(overallStr);
            MediaItem::trim(seasonStr);
            MediaItem::trim(inSeasonStr);
            MediaItem::trim(airDate);
            MediaItem::trim(lengthStr);
            MediaItem::trim(title);
            MediaItem::trim(summary);

            if (id.empty() || overallStr.empty() || inSeasonStr.empty() || lengthStr.empty())
                throw "Not a valid episode.";

            unsigned short overall = static_cast<unsigned short>(std::stoul(overallStr));
            unsigned short season;
            if (seasonStr.empty())
                season = 1u; 
            else
                season = static_cast<unsigned short>(std::stoul(seasonStr));

            unsigned short inSeason = static_cast<unsigned short>(std::stoul(inSeasonStr));
            unsigned int lengthSec = parseLengthToSeconds(lengthStr);

            TvShow* showPtr = nullptr;
            for (size_t i = 0; i < col.size(); ++i)
            {
                auto item = col[i];
                auto tv = dynamic_cast<TvShow*>(item);
                if (tv && tv->m_id == id)
                {
                    showPtr = tv;
                    break;
                }
            }

            if (!showPtr)
            {
                throw std::string("Episode cannot be matched to any show in the collection.");
            }

            TvEpisode ep;
            ep.m_show = showPtr;
            ep.m_numberOverall = overall;
            ep.m_season = season;
            ep.m_numberInSeason = inSeason;
            ep.m_airDate = airDate;
            ep.m_length = lengthSec;
            ep.m_title = title;
            ep.m_summary = summary;

            showPtr->m_episodes.push_back(ep);
        }

        double getEpisodeAverageLength() const;
        std::list<std::string> getLongEpisodes() const;
    };
}

#endif
