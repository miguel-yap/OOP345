/* Citation and Sources.... 
Assignment 04
Module: Workstation
FileName: Workstation.cpp
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 30, 2025
------------------------------------------------------------------
- Line 34-59 was assisted with the help of ChatGPT.
- Line 70-78 was assisted with the help of ChatGPT.
------------------------------------------------------------------*/

#include <iostream>
#include <iomanip>
#include "Workstation.h"
#include "CustomerOrder.h"

namespace seneca {
    std::deque<CustomerOrder> g_pending;
    std::deque<CustomerOrder> g_completed;
    std::deque<CustomerOrder> g_incomplete;

    Workstation::Workstation(const std::string& record) : Station(record) {}

    void Workstation::fill(std::ostream& os) {
        if (!m_orders.empty()) {
            m_orders.front().fillItem(*this, os);
        }
    }
    
    // The following display function implementation was developed with guidance from ChatGPT.
    bool Workstation::attemptToMoveOrder() {
        if (m_orders.empty()) {
            return false;
        }

        CustomerOrder& currentOrder = m_orders.front();
        bool filledAtThisStation = currentOrder.isItemFilled(getItemName());

        if (filledAtThisStation || getQuantity() == 0) {
            
            if (m_pNextStation) {
                *m_pNextStation += std::move(currentOrder);
            } else {
                if (currentOrder.isOrderFilled()) {
                    g_completed.push_back(std::move(currentOrder));
                } else {
                    g_incomplete.push_back(std::move(currentOrder));
                }
            }
            
            m_orders.pop_front();
            return true;
        }

        return false; 
    }

    void Workstation::setNextStation(Workstation* station) {
        m_pNextStation = station;
    }

    Workstation* Workstation::getNextStation() const {
        return m_pNextStation;
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    void Workstation::display(std::ostream& os) const {
        os << getItemName() << " --> ";
        if (m_pNextStation) {
            os << m_pNextStation->getItemName();
        } else {
            os << "End of Line";
        }
        os << std::endl;
    }

    Workstation& Workstation::operator+=(CustomerOrder&& newOrder) {
        m_orders.push_back(std::move(newOrder));
        return *this;
    }
}