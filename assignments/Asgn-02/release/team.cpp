#include "team.h"

namespace seneca {

void Team::copyFrom(const Team& other) {
    m_name = other.m_name;
    m_count = other.m_count;
    if (m_count > 0) {
        m_members = new Character*[m_count];
        for (size_t i = 0; i < m_count; ++i)
            m_members[i] = other.m_members[i]->clone();
    }
}

void Team::clear() {
    for (size_t i = 0; i < m_count; ++i)
        delete m_members[i];
    delete[] m_members;
    m_members = nullptr;
    m_count = 0;
}

Team::Team() = default;

Team::Team(const char* name) : m_name(name) {}

Team::Team(const Team& other) {
    copyFrom(other);
}

Team& Team::operator=(const Team& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

Team::Team(Team&& other) noexcept {
    *this = std::move(other);
}

Team& Team::operator=(Team&& other) noexcept {
    if (this != &other) {
        clear();
        m_name = std::move(other.m_name);
        m_members = other.m_members;
        m_count = other.m_count;
        other.m_members = nullptr;
        other.m_count = 0;
    }
    return *this;
}

Team::~Team() {
    clear();
}

void Team::addMember(const Character* c) {
    if (!c)
        return;

    // prevent duplicates
    for (size_t i = 0; i < m_count; ++i)
        if (m_members[i]->getName() == c->getName())
            return;

    Character** newArr = new Character*[m_count + 1];
    for (size_t i = 0; i < m_count; ++i)
        newArr[i] = m_members[i];
    newArr[m_count] = c->clone();

    delete[] m_members;
    m_members = newArr;
    ++m_count;
}

void Team::removeMember(const std::string& name) {
    for (size_t i = 0; i < m_count; ++i) {
        if (m_members[i]->getName() == name) {
            delete m_members[i];
            for (size_t j = i; j < m_count - 1; ++j)
                m_members[j] = m_members[j + 1];
            --m_count;
            if (m_count == 0) {
                delete[] m_members;
                m_members = nullptr;
            }
            return;
        }
    }
}

Character* Team::operator[](size_t idx) const {
    if (idx < m_count)
        return m_members[idx];
    return nullptr;
}

void Team::showMembers() const {
    if (m_name.empty() && m_count == 0) {
        std::cout << "No team." << std::endl;
        return;
    }

    std::cout << "[Team] " << m_name << std::endl;
    for (size_t i = 0; i < m_count; ++i) {
        std::cout << "    " << (i + 1) << ": " << *m_members[i] << std::endl;
    }
}

} // namespace seneca
