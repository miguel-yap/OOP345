/* 
The function ostream& operator<<(std::ostream& os, const Event& e) (lines 12) was generated with the help of ChatGPT.
*/

#include "event.h"
#include "settings.h"
#include <iomanip>

namespace seneca {

    std::ostream& operator<<(std::ostream& os, const Event& e) {
        static unsigned counter = 0;
        ++counter;

        int width = 11; 
        if (g_settings.m_time_units == "seconds") {
            width = 2;
        } else if (g_settings.m_time_units == "milliseconds") {
            width = 5;
        } else if (g_settings.m_time_units == "microseconds") {
            width = 8;
        }

        long long value{};
        if (g_settings.m_time_units == "seconds") {
            value = std::chrono::duration_cast<std::chrono::seconds>(e.m_duration).count();
        } else if (g_settings.m_time_units == "milliseconds") {
            value = std::chrono::duration_cast<std::chrono::milliseconds>(e.m_duration).count();
        } else if (g_settings.m_time_units == "microseconds") {
            value = std::chrono::duration_cast<std::chrono::microseconds>(e.m_duration).count();
        } else { 
            value = e.m_duration.count();
        }

        os << std::right << std::setw(2) << counter << ": "
           << std::right << std::setw(40) << e.m_name << " -> "
           << std::right << std::setw(width) << value << ' '
           << g_settings.m_time_units;

        return os;
    }

} // namespace seneca
