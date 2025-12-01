/* Citation and Sources.... 
Assignment 04
Module: Station
FileName: Station.cpp
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 30, 2025
------------------------------------------------------------------
- Line 27-59 was assisted with the help of ChatGPT.
- Line 80-92 was assisted with the help of ChatGPT.
------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>
#include <string>
#include "Station.h"
#include "Utilities.h"

namespace seneca {
    size_t Station::m_widthField = 0;
    size_t Station::id_generator = 1;

    // The following display function implementation was developed with guidance from ChatGPT.
    Station::Station(const std::string& record) {
        Utilities util;
        size_t next_pos = 0;
        bool more = true;

        m_id = id_generator++;

        m_itemName = util.extractToken(record, next_pos, more);

        std::string serialStr = util.extractToken(record, next_pos, more);
        try {
            m_serialNumber = std::stoul(serialStr);
        } catch (...) {
            m_serialNumber = 0;
        }

        std::string qtyStr = util.extractToken(record, next_pos, more);
        try {
            m_stockQuantity = std::stoul(qtyStr);
        } catch (...) {
            m_stockQuantity = 0;
        }

        if (util.getFieldWidth() > m_widthField) {
            m_widthField = util.getFieldWidth();
        }

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

    // The following display function implementation was developed with guidance from ChatGPT.
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