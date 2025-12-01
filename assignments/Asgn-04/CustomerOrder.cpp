#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <utility>
#include <string> // Added string for clarity
#include "CustomerOrder.h"
#include "Utilities.h"
#include "Station.h" // Added Station.h to ensure Station class is visible

using namespace std;

// Assuming your namespace is 'sdds' based on your provided files
namespace sdds {

    size_t CustomerOrder::m_widthField = 0;

    CustomerOrder::CustomerOrder() :
        m_name{}, m_item{}, m_cntItem{}, m_lstItem{ nullptr } {
    }

    CustomerOrder::CustomerOrder(const string& input) {
        Utilities utility;
        size_t pos = 0;
        bool more = true;

        // Use a local string for the record to ensure the original input isn't modified
        string tempInput = input; 

        // 1. Extract Customer Name
        m_name = utility.extractToken(tempInput, pos, more);
        
        // 2. Extract Product Name
        m_item = utility.extractToken(tempInput, pos, more);

        // 3. Extract all remaining items
        vector<string> items;
        while (more) {
            string token = utility.extractToken(tempInput, pos, more);
            if (!token.empty()) {
                items.push_back(token);
            }
        }
        
        m_cntItem = items.size();
        m_lstItem = new Item * [m_cntItem];

        for (size_t i = 0; i < m_cntItem; ++i)
            m_lstItem[i] = new Item(items[i]);

        if (m_widthField < utility.getFieldWidth()) {
            m_widthField = utility.getFieldWidth();
        }
    }

    CustomerOrder::CustomerOrder(const CustomerOrder& order) {
        throw string("Copy constructor cannot be called");
    }

    CustomerOrder::CustomerOrder(CustomerOrder&& order) noexcept :
        m_name{}, m_item{}, m_cntItem{}, m_lstItem{ nullptr } {
        *this = move(order);
    }

    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& order) noexcept {
        if (this == &order) {
            return *this;
        }

        // Clean up current object before swapping (essential for safety)
        if (m_lstItem) {
            for (size_t i = 0; i < m_cntItem; i++) {
                delete m_lstItem[i];
            }
            delete[] m_lstItem;
        }

        m_name = move(order.m_name);
        m_item = move(order.m_item);
        m_cntItem = order.m_cntItem;
        m_lstItem = order.m_lstItem; // Steal resource

        order.m_cntItem = 0u;
        order.m_lstItem = nullptr; // Nullify resource pointer in source

        return *this;
    }

    CustomerOrder::~CustomerOrder() {
        for (size_t i = 0; i < m_cntItem; i++) {
            delete m_lstItem[i];
        }
        delete[] m_lstItem;
    }

    bool CustomerOrder::isOrderFilled() const {
        for (size_t i = 0; i < m_cntItem; i++) {
            if (!m_lstItem[i]->m_isFilled) {
                return false;
            }
        }
        return true;
    }

    bool CustomerOrder::isItemFilled(const string& itemName) const {
        size_t itemCount = 0;
        size_t filledCount = 0;

        for (size_t i = 0; i < m_cntItem; i++) {
            if (m_lstItem[i]->m_itemName == itemName) {
                itemCount++;

                if (m_lstItem[i]->m_isFilled) {
                    filledCount++;
                }
            }
        }
        // Returns true if all requested items of that name are filled, or if the item wasn't requested (itemCount == filledCount == 0)
        return itemCount == filledCount;
    }

    void CustomerOrder::fillItem(Station& station, ostream& os) {
        for (size_t i = 0; i < m_cntItem; i++) {

            if (m_lstItem[i]->m_isFilled == false && m_lstItem[i]->m_itemName == station.getItemName()) {

                if (station.getQuantity() > 0) {

                    m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
                    m_lstItem[i]->m_isFilled = true;
                    station.updateQuantity();
                    
                    // FIX 1: Use explicit four spaces for required indentation. Ditch setw/right.
                    os << "    Filled " << m_name << ", " << m_item 
                       << " [" << m_lstItem[i]->m_itemName << "]" << endl;
                    break; // Fill only ONE item per fill call

                }
                else {

                    // FIX 1: Use explicit four spaces for required indentation.
                    os << "    Unable to fill " << m_name << ", " << m_item 
                       << " [" << m_lstItem[i]->m_itemName << "]" << endl;
                    
                    // Do NOT break here. You must check other items if the current one is out of stock.
                    // Wait, specification says: "fills one item... if there is any such request."
                    // If the first unfilled item cannot be filled, we report and stop for this station/order.
                    break; 

                }
            }
        }
    }

    void CustomerOrder::display(ostream& os) const {
        os << m_name << " - " << m_item << endl;

        for (size_t i = 0; i < m_cntItem; i++) {
            
            os << "[";
            // FIX 2: Explicitly ensure right justification for padded serial number
            os << std::right << setw(6) << setfill('0') << m_lstItem[i]->m_serialNumber; 
            os << "] ";
            
            os << setfill(' ') << left;
            os << setw(m_widthField + 2) << m_lstItem[i]->m_itemName << " - "; // Added +2 for width to match sample
            os << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED");
            os << endl;
        }
    }

}