#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include "CustomerOrder.h"
#include "Utilities.h"

namespace seneca {
    // Initialize the static member
    size_t CustomerOrder::m_widthField = 0;

    CustomerOrder::CustomerOrder() : m_cntItem(0), m_lstItem(nullptr) {}

    CustomerOrder::CustomerOrder(const std::string& record) {
        Utilities util;
        size_t next_pos = 0;
        bool more = true;
        std::vector<Item*> tempItems;

        // 1. Extract Customer Name
        m_name = util.extractToken(record, next_pos, more);

        // 2. Extract Product Name
        m_product = util.extractToken(record, next_pos, more);

        // 3. Extract Items List
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

        // Update m_widthField
        if (util.getFieldWidth() > m_widthField) {
            m_widthField = util.getFieldWidth();
        }
    }

    // Rule of Three/Five: Copy Constructor (throws exception)
    CustomerOrder::CustomerOrder(const CustomerOrder& other) {
        throw "ERROR: Cannot make copies.";
    }

    // Rule of Three/Five: Destructor
    CustomerOrder::~CustomerOrder() {
        for (size_t i = 0; i < m_cntItem; ++i) {
            delete m_lstItem[i];
        }
        delete[] m_lstItem;
    }

    // Rule of Three/Five: Move Constructor
    CustomerOrder::CustomerOrder(CustomerOrder&& other) noexcept : CustomerOrder() {
        *this = std::move(other);
    }

    // Rule of Three/Five: Move Assignment Operator
    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& other) noexcept {
        if (this != &other) {
            // Clean up current object's resources
            for (size_t i = 0; i < m_cntItem; ++i) {
                delete m_lstItem[i];
            }
            delete[] m_lstItem;

            // Transfer ownership
            m_name = std::move(other.m_name);
            m_product = std::move(other.m_product);
            m_cntItem = other.m_cntItem;
            m_lstItem = other.m_lstItem;
            
            // Invalidate the source object
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
        // If the item doesn't exist in the order, return true.
        bool itemExists = false;
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (m_lstItem[i]->m_itemName == itemName) {
                itemExists = true;
                if (!m_lstItem[i]->m_isFilled) {
                    return false; // Found an unfilled item
                }
            }
        }
        return itemExists || !itemExists; // Returns true if no matching item found, or if all matching items are filled
    }

    void CustomerOrder::fillItem(Station& station, std::ostream& os) {
        bool itemFound = false;

        // Iterate through items to find the first matching, unfilled item
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (m_lstItem[i]->m_itemName == station.getItemName() && !m_lstItem[i]->m_isFilled) {
                itemFound = true;
                if (station.getQuantity() > 0) {
                    // Fill the order
                    station.updateQuantity();
                    m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
                    m_lstItem[i]->m_isFilled = true;
                    
                    os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
                    break; // Fill one item and exit
                } else {
                    // Inventory is empty
                    os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
                    break; // Print message and exit (only one fill per item per station is expected in one step)
                }
            }
        }
    }

    void CustomerOrder::display(std::ostream& os) const {
        os << m_name << " - " << m_product << std::endl;
        for (size_t i = 0; i < m_cntItem; ++i) {
            os << "[" << std::right << std::setw(6) << std::setfill('0') << m_lstItem[i]->m_serialNumber << "] ";
            os << std::left << std::setw(m_widthField) << std::setfill(' ') << m_lstItem[i]->m_itemName << " - ";
            os << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << std::endl;
        }
    }
}