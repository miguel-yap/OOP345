#include <iostream>
#include <iomanip>
#include "Workstation.h"
#include "CustomerOrder.h"

namespace seneca {
    // Define global queues
    std::deque<CustomerOrder> g_pending;
    std::deque<CustomerOrder> g_completed;
    std::deque<CustomerOrder> g_incomplete;

    Workstation::Workstation(const std::string& record) : Station(record) {}

    void Workstation::fill(std::ostream& os) {
        if (!m_orders.empty()) {
            // Fills one item in the order at the front of the queue
            m_orders.front().fillItem(*this, os);
        }
    }

    bool Workstation::attemptToMoveOrder() {
        if (m_orders.empty()) {
            return false;
        }

        CustomerOrder& currentOrder = m_orders.front();
        bool filledAtThisStation = currentOrder.isItemFilled(getItemName());

        // An order MUST move if:
        // 1. It is fully filled for the item at this station (filledAtThisStation == true), OR
        // 2. The station is out of stock (getQuantity() == 0). This prevents the infinite loop.
        // If an order is not filled, and stock is 0, it must move to mark the item incomplete later.
        if (filledAtThisStation || getQuantity() == 0) {
            
            // 1. Move to Next Station or Final Queue
            if (m_pNextStation) {
                *m_pNextStation += std::move(currentOrder);
            } else {
                // End of the line.
                if (currentOrder.isOrderFilled()) {
                    g_completed.push_back(std::move(currentOrder));
                } else {
                    g_incomplete.push_back(std::move(currentOrder));
                }
            }
            
            // 2. Remove from current queue
            m_orders.pop_front();
            return true;
        }

        // Otherwise, hold the order back (it still needs service and we still have stock).
        return false; 
    }

    void Workstation::setNextStation(Workstation* station) {
        m_pNextStation = station;
    }

    Workstation* Workstation::getNextStation() const {
        return m_pNextStation;
    }

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