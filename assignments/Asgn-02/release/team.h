#ifndef SENECA_TEAM_H
#define SENECA_TEAM_H

#include "character.h"
#include <string>
#include <iostream>

namespace seneca {

class Team {
    std::string m_name{};
    Character** m_members{};
    size_t m_count{};

    void copyFrom(const Team& other);
    void clear();

public:
    Team();
    Team(const char* name);
    Team(const Team& other);
    Team& operator=(const Team& other);
    Team(Team&& other) noexcept;
    Team& operator=(Team&& other) noexcept;
    ~Team();

    void addMember(const Character* c);
    void removeMember(const std::string& name);
    Character* operator[](size_t idx) const;
    void showMembers() const;
};

} // namespace seneca

#endif
