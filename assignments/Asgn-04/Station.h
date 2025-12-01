/* Citation and Sources.... 
Assignment 04
Module: Station
FileName: Station.h
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 30, 2025
------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. 
This submitted piece of work has not been shared with any other student or 3rd party content provider.
------------------------------------------------------------------*/
#ifndef SENECA_STATION_H
#define SENECA_STATION_H

#include <iostream>
#include <string>

namespace seneca {
    class Station {
        size_t m_id;
        std::string m_itemName;
        std::string m_description;
        size_t m_serialNumber;
        size_t m_stockQuantity;

        static size_t m_widthField; 
        static size_t id_generator; 

    public:
        Station(const std::string& record);
        const std::string& getItemName() const;
        size_t getNextSerialNumber();
        size_t getQuantity() const;
        void updateQuantity();
        void display(std::ostream& os, bool full) const;
    };
}

#endif // SENECA_STATION_H