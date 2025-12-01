#include "Workstation.h"
#include <utility>

namespace seneca {

    std::deque<CustomerOrder> g_pending;
    std::deque<CustomerOrder> g_completed;
    std::deque<CustomerOrder> g_incomplete;

    Workstation::Workstation(const std::string& record)
        : Station(record) {}

    void Workstation::fill(std::ostream& os) {
        if (!m_orders.empty()) {
            m_orders.front().fillItem(*this, os);
        }
    }

    bool Workstation::attemptToMoveOrder() {
    if (m_orders.empty())
        return false;

    CustomerOrder& order = m_orders.front();

    // Does this workstation's item still need to be filled?
    bool needsThisItem = false;
    for (size_t i = 0; i < order.m_cntItem; ++i) {
        if (order.m_lstItem[i]->m_itemName == getItemName() &&
            !order.m_lstItem[i]->m_isFilled) {
            needsThisItem = true;
            break;
        }
    }

    // Move conditions:
    // 1) order does NOT need this item anymore (fill() solved it)
    // 2) OR station CANNOT fill (quantity == 0)
    if (!needsThisItem || getQuantity() == 0) {
        if (m_pNextStation) {
            *m_pNextStation += std::move(order);
        } else {
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

} // namespace seneca
