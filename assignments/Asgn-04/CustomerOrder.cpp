#include "CustomerOrder.h"
#include "Station.h"
#include "Utilities.h"
#include <vector>
#include <iomanip>
#include <stdexcept>

namespace seneca {

    size_t CustomerOrder::m_widthField = 0u;

    CustomerOrder::CustomerOrder(const std::string& record) {
        Utilities util;
        bool more = true;
        size_t pos = 0u;

        m_name = util.extractToken(record, pos, more);
        if (more)
            m_product = util.extractToken(record, pos, more);

        std::vector<std::string> items;
        while (more) {
            items.push_back(util.extractToken(record, pos, more));
        }

        m_cntItem = items.size();
        m_lstItem = new Item*[m_cntItem];
        for (size_t i = 0u; i < m_cntItem; ++i) {
            m_lstItem[i] = new Item(items[i]);
        }

        if (util.getFieldWidth() > m_widthField) {
            m_widthField = util.getFieldWidth();
        }
    }

    // copy constructor must throw (as per spec / tester)
    CustomerOrder::CustomerOrder(const CustomerOrder& other) {
        throw std::runtime_error("Cannot make copies");
    }

    CustomerOrder::CustomerOrder(CustomerOrder&& other) noexcept {
        *this = std::move(other);
    }

    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& other) noexcept {
        if (this != &other) {
            // cleanup current
            if (m_lstItem) {
                for (size_t i = 0u; i < m_cntItem; ++i) {
                    delete m_lstItem[i];
                }
                delete[] m_lstItem;
            }

            m_name = std::move(other.m_name);
            m_product = std::move(other.m_product);
            m_cntItem = other.m_cntItem;
            m_lstItem = other.m_lstItem;

            other.m_cntItem = 0u;
            other.m_lstItem = nullptr;
        }
        return *this;
    }

    CustomerOrder::~CustomerOrder() {
        if (m_lstItem) {
            for (size_t i = 0u; i < m_cntItem; ++i) {
                delete m_lstItem[i];
            }
            delete[] m_lstItem;
        }
    }

    bool CustomerOrder::isOrderFilled() const {
        for (size_t i = 0u; i < m_cntItem; ++i) {
            if (!m_lstItem[i]->m_isFilled) {
                return false;
            }
        }
        return true;
    }

    bool CustomerOrder::isItemFilled(const std::string& itemName) const {
    bool contains = false;

    for (size_t i = 0; i < m_cntItem; ++i) {
        if (m_lstItem[i]->m_itemName == itemName) {
            contains = true;
            if (!m_lstItem[i]->m_isFilled)
                return false;
        }
    }

    // if item not in order → considered filled
    return true;
}


    void CustomerOrder::fillItem(Station& station, std::ostream& os) {
        for (size_t i = 0u; i < m_cntItem; ++i) {
            if (m_lstItem[i]->m_itemName == station.getItemName() &&
                !m_lstItem[i]->m_isFilled) {

                if (station.getQuantity() > 0u) {
                    m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
                    m_lstItem[i]->m_isFilled = true;
                    station.updateQuantity();

                    os << "    Filled " << m_name << ", " << m_product
                       << " [" << station.getItemName() << "]\n";
                } else {
                    os << "    Unable to fill " << m_name << ", " << m_product
                       << " [" << station.getItemName() << "]\n";
                }
                // only first matching unfilled item processed
                break;
            }
        }
    }

    void CustomerOrder::display(std::ostream& os) const {
        os << m_name << " - " << m_product << '\n';
        for (size_t i = 0u; i < m_cntItem; ++i) {
            os << "[";
            os << std::setw(6) << std::setfill('0') << m_lstItem[i]->m_serialNumber;
            os << "] ";
            os << std::left << std::setw(m_widthField) << std::setfill(' ')
               << m_lstItem[i]->m_itemName << " - ";
            os << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << '\n';
        }
    }

} // namespace seneca
