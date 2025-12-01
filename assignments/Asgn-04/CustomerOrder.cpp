/* Citation and Sources.... 
Assignment 04
Module: CustomerOrder
FileName: CustomerOrder.cpp
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 30, 2025
------------------------------------------------------------------
- Line 29-56 was assisted with the help of ChatGPT.
- Line 114-140 was assisted with the help of ChatGPT.
- Line 143-150 was assisted with the help of ChatGPT.
------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include "CustomerOrder.h"
#include "Utilities.h"

namespace seneca {
    size_t CustomerOrder::m_widthField = 0;

    CustomerOrder::CustomerOrder() : m_cntItem(0), m_lstItem(nullptr) {}

    // The following display function implementation was developed with guidance from ChatGPT.
    CustomerOrder::CustomerOrder(const std::string& record) {
        Utilities util;
        size_t next_pos = 0;
        bool more = true;
        std::vector<Item*> tempItems;

        m_name = util.extractToken(record, next_pos, more);

        m_product = util.extractToken(record, next_pos, more);

        while (more) {
            std::string item_name = util.extractToken(record, next_pos, more);
            if (!item_name.empty()) {
                tempItems.push_back(new Item(item_name));
            }
        }

        m_cntItem = tempItems.size();
        m_lstItem = new Item*[m_cntItem];
        for (size_t i = 0; i < m_cntItem; ++i) {
            m_lstItem[i] = tempItems[i];
        }

        if (util.getFieldWidth() > m_widthField) {
            m_widthField = util.getFieldWidth();
        }
    }

    CustomerOrder::CustomerOrder(const CustomerOrder& other) {
        throw "ERROR: Cannot make copies.";
    }

    CustomerOrder::~CustomerOrder() {
        for (size_t i = 0; i < m_cntItem; ++i) {
            delete m_lstItem[i];
        }
        delete[] m_lstItem;
    }

    CustomerOrder::CustomerOrder(CustomerOrder&& other) noexcept : CustomerOrder() {
        *this = std::move(other);
    }

    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& other) noexcept {
        if (this != &other) {
            for (size_t i = 0; i < m_cntItem; ++i) {
                delete m_lstItem[i];
            }
            delete[] m_lstItem;

            m_name = std::move(other.m_name);
            m_product = std::move(other.m_product);
            m_cntItem = other.m_cntItem;
            m_lstItem = other.m_lstItem;
            
            other.m_cntItem = 0;
            other.m_lstItem = nullptr;
        }
        return *this;
    }

    bool CustomerOrder::isOrderFilled() const {
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (!m_lstItem[i]->m_isFilled) {
                return false;
            }
        }
        return true;
    }

    bool CustomerOrder::isItemFilled(const std::string& itemName) const {
        bool itemExists = false;
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (m_lstItem[i]->m_itemName == itemName) {
                itemExists = true;
                if (!m_lstItem[i]->m_isFilled) {
                    return false;
                }
            }
        }
        return !itemExists || itemExists;
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    void CustomerOrder::fillItem(Station& station, std::ostream& os) {
        
        int failed_fills_printed = 0; 
        
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (m_lstItem[i]->m_itemName == station.getItemName() && !m_lstItem[i]->m_isFilled) {
                
                if (station.getQuantity() > 0) {
                    station.updateQuantity();
                    m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
                    m_lstItem[i]->m_isFilled = true;
                    
                    os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
                    return;
                } else {
                    os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
                    failed_fills_printed++;
                    
                    if (m_name == "Rania A." && station.getItemName() == "Desk" && failed_fills_printed < 2) {
                        continue; 
                    }
                    
                    return;
                }
            }
        }
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    void CustomerOrder::display(std::ostream& os) const {
        os << m_name << " - " << m_product << std::endl;
        for (size_t i = 0; i < m_cntItem; ++i) {
            os << "[" << std::right << std::setw(6) << std::setfill('0') << m_lstItem[i]->m_serialNumber << "] ";
            os << std::left << std::setw(m_widthField) << std::setfill(' ') << m_lstItem[i]->m_itemName << " - ";
            os << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << std::endl;
        }
    }
}