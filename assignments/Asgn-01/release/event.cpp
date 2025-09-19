#include "event.h"
#include "settings.h"
#include <iomanip>

namespace seneca {

    std::ostream& operator<<(std::ostream& os, const Event& e) {
        // Local static counter (persists across calls)
        static unsigned counter = 0;
        ++counter;

        // Width of duration field depends on g_settings.m_time_units
        int width = 11; // default nanoseconds
        if (g_settings.m_time_units == "seconds") {
            width = 2;
        } else if (g_settings.m_time_units == "milliseconds") {
            width = 5;
        } else if (g_settings.m_time_units == "microseconds") {
            width = 8;
        }

        // Convert duration into correct unit
        long long value{};
        if (g_settings.m_time_units == "seconds") {
            value = std::chrono::duration_cast<std::chrono::seconds>(e.m_duration).count();
        } else if (g_settings.m_time_units == "milliseconds") {
            value = std::chrono::duration_cast<std::chrono::milliseconds>(e.m_duration).count();
        } else if (g_settings.m_time_units == "microseconds") {
            value = std::chrono::duration_cast<std::chrono::microseconds>(e.m_duration).count();
        } else { // nanoseconds
            value = e.m_duration.count();
        }

        // Print according to required format
        os << std::right << std::setw(2) << counter << ": "
           << std::right << std::setw(40) << e.m_name << " -> "
           << std::right << std::setw(width) << value << ' '
           << g_settings.m_time_units;

        return os;
    }

} // namespace seneca
