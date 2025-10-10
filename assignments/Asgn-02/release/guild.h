#ifndef SENECA_GUILD_H
#define SENECA_GUILD_H

#include "character.h"
#include <string>
#include <iostream>

namespace seneca {

class Guild {
    std::string m_name{};
    Character** m_members{};
    size_t m_count{};

    void copyFrom(const Guild& other);
    void clear();

public:
    Guild();
    Guild(const char* name);
    Guild(const Guild& other);
    Guild& operator=(const Guild& other);
    Guild(Guild&& other) noexcept;
    Guild& operator=(Guild&& other) noexcept;
    ~Guild();

    void addMember(Character* c);
    void removeMember(const std::string& name);
    Character* operator[](size_t idx) const;
    void showMembers() const;
};

} // namespace seneca

#endif
