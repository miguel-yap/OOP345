#ifndef SENECA_EVENT_H
#define SENECA_EVENT_H

#include <string>
#include <chrono>
#include <ostream>

namespace seneca {
    class Event {
        private:
            std::string m_name{};
            std::chrono::nanoseconds m_duration{};
        
        public:
            Event() = default;
            Event(const char* name, const std::chrono::nanoseconds& duration) : m_name{name ? name:""}, m_duration{duration} {};

            friend std::ostream& operator<<(std::ostream& os, const Event& e);

    };
}

#endif // SENECA_EVENT_H