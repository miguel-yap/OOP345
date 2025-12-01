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

        // Condition 1: Item is filled at this station, OR
        // Condition 2: We are at the end of the line.
        if (filledAtThisStation) {
            // The item required by this station is fully filled for this order. Move it.
            if (m_pNextStation) {
                *m_pNextStation += std::move(currentOrder);
            } else {
                // End of line. Check completion status.
                if (currentOrder.isOrderFilled()) {
                    g_completed.push_back(std::move(currentOrder));
                } else {
                    g_incomplete.push_back(std::move(currentOrder));
                }
            }
            
            m_orders.pop_front();
            return true;
        } else if (m_pNextStation == nullptr) {
            // At the end of the line, and the item is NOT filled. Must move it to g_incomplete.
            g_incomplete.push_back(std::move(currentOrder));
            m_orders.pop_front();
            return true;
        }

        return false; // Order needs service and has a next station, so hold it.
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