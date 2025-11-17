#ifndef SENECA_MOVIE_H
#define SENECA_MOVIE_H

#include <string>
#include "mediaItem.h"
#include "settings.h"

namespace seneca
{
    class Movie : public MediaItem
    {
        // No extra data needed beyond MediaItem for this assignment

        Movie(const std::string& title,
              unsigned short year,
              const std::string& summary);

    public:
        void display(std::ostream& out = std::cout) const override;

        static Movie* createItem(const std::string& strMovie);
    };
}

#endif
