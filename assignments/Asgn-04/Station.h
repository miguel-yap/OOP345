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

        static size_t m_widthField; // Max width of item names
        static size_t id_generator; // Generates unique IDs

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