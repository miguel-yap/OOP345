/* 
The function void TimeMonitor::startEvent(const char* name) (lines 11-15) was generated with the help of ChatGPT.
The function Event TimeMonitor::stopEvent() (line 17-22) was generated with the help of ChatGPT.
*/

#include "timeMonitor.h"
#include <utility>

namespace seneca {

void TimeMonitor::startEvent(const char* name) {
    m_currentName = name ? name : "";
    m_start = clock::now();
    m_running = true;
}

Event TimeMonitor::stopEvent() {
    auto end = clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_start);
    m_running = false;
    return Event(m_currentName.c_str(), dur);
}

} // namespace seneca
