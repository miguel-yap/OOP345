/* Citation and Sources.... 
Assignment 03
Module: movie
FileName: movie.h
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 16, 2025
------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. 
This submitted piece of work has not been shared with any other student or 3rd party content provider.
------------------------------------------------------------------*/
#ifndef SENECA_MOVIE_H
#define SENECA_MOVIE_H

#include <string>
#include "mediaItem.h"
#include "settings.h"

namespace seneca
{
    class Movie : public MediaItem
    {

        Movie(const std::string& title,
              unsigned short year,
              const std::string& summary);

    public:
        void display(std::ostream& out = std::cout) const override;

        static Movie* createItem(const std::string& strMovie);
    };
}

#endif
