#include <iostream>
#include <iomanip>
#include <string>
#include "Station.h"
#include "Utilities.h"

namespace seneca {
    // Initialize the static members
    size_t Station::m_widthField = 0;
    size_t Station::id_generator = 1; // Starts at 1

    Station::Station(const std::string& record) {
        Utilities util;
        size_t next_pos = 0;
        bool more = true;

        // 1. Assign unique ID and increment generator
        m_id = id_generator++;

        // 2. Extract Item Name
        m_itemName = util.extractToken(record, next_pos, more);

        // 3. Extract Starting Serial Number
        std::string serialStr = util.extractToken(record, next_pos, more);
        try {
            m_serialNumber = std::stoul(serialStr);
        } catch (...) {
            // Handle error if conversion fails (though unlikely with valid data)
            m_serialNumber = 0;
        }

        // 4. Extract Quantity in Stock
        std::string qtyStr = util.extractToken(record, next_pos, more);
        try {
            m_stockQuantity = std::stoul(qtyStr);
        } catch (...) {
            m_stockQuantity = 0;
        }

        // 5. Update m_widthField
        if (util.getFieldWidth() > m_widthField) {
            m_widthField = util.getFieldWidth();
        }

        // 6. Extract Description (remaining part of the string)
        if (more) {
            m_description = util.extractToken(record, next_pos, more);
        } else {
            m_description = "";
        }
    }

    const std::string& Station::getItemName() const {
        return m_itemName;
    }

    size_t Station::getNextSerialNumber() {
        return m_serialNumber++;
    }

    size_t Station::getQuantity() const {
        return m_stockQuantity;
    }

    void Station::updateQuantity() {
        if (m_stockQuantity > 0) {
            m_stockQuantity--;
        }
    }

    void Station::display(std::ostream& os, bool full) const {
        os << std::right << std::setw(3) << std::setfill('0') << m_id << " | ";
        os << std::left << std::setw(m_widthField) << std::setfill(' ') << m_itemName << " | ";
        
        os << std::right << std::setw(6) << std::setfill('0') << m_serialNumber << " | ";

        if (full) {
            os << std::right << std::setw(4) << std::setfill(' ') << m_stockQuantity << " | ";
            os << m_description;
        }

        os << std::endl;
    }
}