#include "Workstation.h"
#include <utility>
#include <algorithm> // Not strictly necessary here but good practice

namespace seneca {

    std::deque<CustomerOrder> g_pending;
    std::deque<CustomerOrder> g_completed;
    std::deque<CustomerOrder> g_incomplete;

    Workstation::Workstation(const std::string& record)
        : Station(record) {}

    void Workstation::fill(std::ostream& os) {
        if (!m_orders.empty()) {
            // Fill the order at the front of the queue, if possible
            m_orders.front().fillItem(*this, os);
        }
    }

    bool Workstation::attemptToMoveOrder() {
        if (m_orders.empty())
            return false;

        CustomerOrder& order = m_orders.front();

        // Condition for moving:
        // 1. The item needed from this station is already filled (requires no more service).
        // 2. The item is needed, but the station is out of stock (cannot be filled).
        
        bool canMove = false;
        
        // isItemFilled returns true if all required items of this type are filled 
        // OR if the item is not requested in the order at all.
        if (order.isItemFilled(getItemName()) || getQuantity() == 0) {
            canMove = true;
        }

        if (canMove) {
            if (m_pNextStation) {
                // Move order to the next workstation
                *m_pNextStation += std::move(order);
            } 
            else {
                // End of Line: move to completed/incomplete global queues
                if (order.isOrderFilled())
                    g_completed.push_back(std::move(order));
                else
                    g_incomplete.push_back(std::move(order));
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

    void Workstation::display(std::ostream& os) const {
        os << getItemName() << " --> ";
        if (m_pNextStation) {
            os << m_pNextStation->getItemName();
        } else {
            os << "End of Line";
        }
        os << '\n';
    }

    Workstation& Workstation::operator+=(CustomerOrder&& newOrder) {
        m_orders.push_back(std::move(newOrder));
        return *this;
    }

}