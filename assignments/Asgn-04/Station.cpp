#include "Station.h"
#include "Utilities.h"
#include <iomanip>

namespace seneca {

    size_t Station::m_widthField = 0u;
    int Station::id_generator = 0;

    Station::Station(const std::string& record) {
        Utilities util;
        bool more = true;
        size_t pos = 0u;

        m_id = ++id_generator;

        // 1) Item name
        m_itemName = util.extractToken(record, pos, more);

        // IMPORTANT: widthField should be based ONLY on the item name
        if (util.getFieldWidth() > m_widthField) {
            m_widthField = util.getFieldWidth();
        }

        // 2) Serial number
        if (more) {
            std::string token = util.extractToken(record, pos, more);
            m_serialNumber = static_cast<size_t>(std::stoul(token));
        }

        // 3) Quantity
        if (more) {
            std::string token = util.extractToken(record, pos, more);
            m_quantity = static_cast<size_t>(std::stoul(token));
        }

        // 4) Description
        if (more) {
            m_description = util.extractToken(record, pos, more);
        }
    }

    const std::string& Station::getItemName() const {
        return m_itemName;
    }

    size_t Station::getNextSerialNumber() {
        return m_serialNumber++;
    }

    size_t Station::getQuantity() const {
        return m_quantity;
    }

    void Station::updateQuantity() {
        if (m_quantity > 0u) {
            --m_quantity;
        }
    }

    void Station::display(std::ostream& os, bool full) const {
        os << std::setw(3) << std::setfill('0') << m_id << " | ";
        os << std::left << std::setw(m_widthField) << std::setfill(' ') << m_itemName << " | ";
        os << std::right << std::setw(6) << std::setfill('0') << m_serialNumber << " | ";

        if (full) {
            os << std::setfill(' ') << std::setw(4) << m_quantity << " | ";
            os << m_description;
        }

        os << '\n';
    }

} // namespace seneca
