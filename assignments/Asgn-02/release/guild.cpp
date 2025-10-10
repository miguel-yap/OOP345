/* Citation and Sources...
Assignment 02
Module: guild
Filename: guild.cpp
Author: Miguel Yap, StNo: 168049237, Email: myap1@myseneca.ca
-----------------------------------------------------------
- Line 51-61 was assisted with the help of ChatGPT.
- Line 69-87 was assisted with the help of ChatGPT.
- Line 91-107 was assisted with the help of ChatGPT.
-----------------------------------------------------------*/
#include "guild.h"
#include <algorithm>

namespace seneca {

void Guild::copyFrom(const Guild& other) {
    m_name = other.m_name;
    m_count = other.m_count;
    if (m_count > 0) {
        m_members = new Character*[m_count];
        for (size_t i = 0; i < m_count; ++i)
            m_members[i] = other.m_members[i]; 
    }
}

void Guild::clear() {
    delete[] m_members;
    m_members = nullptr;
    m_count = 0;
}

Guild::Guild() = default;

Guild::Guild(const char* name) : m_name(name) {}

Guild::Guild(const Guild& other) {
    copyFrom(other);
}

Guild& Guild::operator=(const Guild& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

Guild::Guild(Guild&& other) noexcept {
    *this = std::move(other);
}

// The following display function implementation was developed with guidance from ChatGPT.
Guild& Guild::operator=(Guild&& other) noexcept {
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

Guild::~Guild() {
    clear(); 
}

// The following display function implementation was developed with guidance from ChatGPT.
void Guild::addMember(Character* c) {
    if (!c)
        return;

    for (size_t i = 0; i < m_count; ++i)
        if (m_members[i]->getName() == c->getName())
            return;

    Character** newArr = new Character*[m_count + 1];
    for (size_t i = 0; i < m_count; ++i)
        newArr[i] = m_members[i];
    newArr[m_count] = c;

    delete[] m_members;
    m_members = newArr;
    ++m_count;

    c->setHealthMax(c->getHealthMax() + 300);
}

// The following display function implementation was developed with guidance from ChatGPT.
void Guild::removeMember(const std::string& name) {
    for (size_t i = 0; i < m_count; ++i) {
        if (m_members[i]->getName() == name) {
            m_members[i]->setHealthMax(m_members[i]->getHealthMax() - 300);

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

Character* Guild::operator[](size_t idx) const {
    if (idx < m_count)
        return m_members[idx];
    return nullptr;
}

void Guild::showMembers() const {
    if (m_name.empty() && m_count == 0) {
        std::cout << "No guild." << std::endl;
        return;
    }

    std::cout << "[Guild] " << m_name << std::endl;
    for (size_t i = 0; i < m_count; ++i) {
        std::cout << "    " << (i + 1) << ": " << *m_members[i] << std::endl;
    }
}

} // namespace seneca
