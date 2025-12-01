/* Citation and Sources.... 
Assignment 04
Module: Utilities
FileName: Utilities.h
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 30, 2025
------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. 
This submitted piece of work has not been shared with any other student or 3rd party content provider.
------------------------------------------------------------------*/
#ifndef SENECA_UTILITIES_H
#define SENECA_UTILITIES_H

#include <string>
#include <iostream>

namespace seneca {
    class Utilities {
        size_t m_widthField{1}; 
        static char m_delimiter; 

    public:
        void setFieldWidth(size_t newWidth);
        size_t getFieldWidth() const;
        std::string extractToken(const std::string& str, size_t& next_pos, bool& more);

        static void setDelimiter(char newDelimiter);
        static char getDelimiter();
    };
}

#endif // SENECA_UTILITIES_H