/* Citation and Sources...
Assignment 02
Module: guild
Filename: guild.h
Author: Miguel Yap, StNo: 168049237, Email: myap1@myseneca.ca
-----------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. 
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------*/
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
