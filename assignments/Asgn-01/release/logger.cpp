/* 
The function void logger::grow(std::size_t newCap) (lines 10-17) was generated with the help of ChatGPT.
The function void logger::addEvent(const Event& event) (line 42-47) was generated with the help of ChatGPT.
*/

#include "logger.h"

namespace seneca {

void Logger::grow(std::size_t newCap) {
    if (newCap <= m_cap) return;
    Event* nxt = new Event[newCap];
    for (std::size_t i = 0; i < m_size; ++i) nxt[i] = m_events[i];
    delete[] m_events;
    m_events = nxt;
    m_cap = newCap;
}

Logger::~Logger() {
    delete[] m_events;
}

Logger::Logger(Logger&& other) noexcept
    : m_events(other.m_events), m_size(other.m_size), m_cap(other.m_cap) {
    other.m_events = nullptr;
    other.m_size = other.m_cap = 0;
}

Logger& Logger::operator=(Logger&& other) noexcept {
    if (this != &other) {
        delete[] m_events;
        m_events = other.m_events;
        m_size = other.m_size;
        m_cap = other.m_cap;
        other.m_events = nullptr;
        other.m_size = other.m_cap = 0;
    }
    return *this;
}

void Logger::addEvent(const Event& event) {
    if (m_size == m_cap) {
        grow(m_cap == 0 ? 2 : m_cap * 2);
    }
    m_events[m_size++] = event;
}

std::ostream& operator<<(std::ostream& os, const Logger& log) {
    for (std::size_t i = 0; i < log.m_size; ++i) {
        os << log.m_events[i] << '\n';
    }
    return os;
}

} // namespace seneca
