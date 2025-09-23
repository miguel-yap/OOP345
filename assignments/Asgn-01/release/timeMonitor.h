/*
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. 
This submitted piece of work has not been shared with any other student or 3rd party content provider.
*/

#ifndef SENECA_TIMEMONITOR_H
#define SENECA_TIMEMONITOR_H

#include "event.h"
#include <chrono>
#include <string>

namespace seneca {

class TimeMonitor {
    using clock = std::chrono::high_resolution_clock;
    std::string m_currentName{};
    clock::time_point m_start{};
    bool m_running{false};

public:
    void startEvent(const char* name);
    Event stopEvent();
};

} // namespace seneca

#endif
