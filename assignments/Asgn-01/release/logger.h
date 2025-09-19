#ifndef SENECA_LOGGER_H
#define SENECA_LOGGER_H

#include "event.h"
#include <cstddef>
#include <iostream>
#include <utility>

namespace seneca {

class Logger {
    Event* m_events{nullptr};
    std::size_t m_size{0};
    std::size_t m_cap{0};

    void grow(std::size_t newCap);

public:
    Logger() = default;
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    Logger(Logger&& other) noexcept;
    Logger& operator=(Logger&& other) noexcept;

    void addEvent(const Event& event);

    friend std::ostream& operator<<(std::ostream& os, const Logger& log);
};

} // namespace seneca

#endif
