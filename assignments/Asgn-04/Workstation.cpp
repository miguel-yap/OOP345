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

        // Get the order at the front
        CustomerOrder& currentOrder = m_orders.front();
        bool filledAtThisStation = currentOrder.isItemFilled(getItemName());
        bool orderFilled = currentOrder.isOrderFilled();

        // Condition 1: Order is fully filled, OR
        // Condition 2: Order cannot be filled (not enough inventory at this station), OR
        // Condition 3: The item is completely filled at this station.
        if (orderFilled || getQuantity() == 0 || filledAtThisStation) {
            
            if (m_pNextStation) {
                // Move to the next workstation
                *m_pNextStation += std::move(currentOrder);
            } else {
                // End of the line
                if (orderFilled) {
                    g_completed.push_back(std::move(currentOrder));
                } else {
                    g_incomplete.push_back(std::move(currentOrder));
                }
            }
            
            // Remove the order from the current queue
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