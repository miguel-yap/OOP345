/* Citation and Sources...
Assignment 02
Module: characterTpl
Filename: characterTpl.h
Author: Miguel Yap, StNo: 168049237, Email: myap1@myseneca.ca
-----------------------------------------------------------
- Line 31-42 was assisted with the help of ChatGPT.
-----------------------------------------------------------*/

#ifndef SENECA_CHARACTERTPL_H
#define SENECA_CHARACTERTPL_H

#include "character.h"
#include <iostream>

namespace seneca {

    template <typename T>
    class CharacterTpl : public Character {
        int m_healthMax{};
        T m_health{};

    public:
        CharacterTpl(const char* name, int healthMax)
            : Character(name), m_healthMax(healthMax), m_health{} {
            m_health = healthMax;
        }

        void takeDamage(int dmg) override {
    m_health -= dmg;

    if (static_cast<int>(m_health) <= 0) {
        // no leading spaces for defeat message
        std::cout << "    " << getName() << " has been defeated!" << std::endl;
    } else {
        // <-- 5 spaces before everything else
        std::cout << "    " << getName()
                  << " took " << dmg
                  << " damage, " << static_cast<int>(m_health)
                  << " health remaining." << std::endl;
    }
}


        int getHealth() const override {
            return static_cast<int>(m_health);
        }

        int getHealthMax() const override {
            return m_healthMax;
        }

        void setHealth(int health) override {
            m_health = health;
        }

        void setHealthMax(int health) override {
            m_healthMax = health;
            m_health = health;
        }
    };

} // namespace seneca

#endif // SENECA_CHARACTERTPL_H
