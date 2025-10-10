/* Citation and Sources...
Assignment 02
Module: rogue
Filename: rogue.h
Author: Miguel Yap, StNo: 168049237, Email: myap1@myseneca.ca
-----------------------------------------------------------
- Line 35-38 was assisted with the help of ChatGPT.
- Line 50-64 was assisted with the help of ChatGPT.
- Line 68-80 was assisted with the help of ChatGPT.
-----------------------------------------------------------*/

#ifndef SENECA_ROGUE_H
#define SENECA_ROGUE_H

#include "characterTpl.h"
#include "weapons.h"
#include <iostream>
#include <algorithm>

namespace seneca {

template <typename T, typename FirstAbility_t, typename SecondAbility_t>
class Rogue : public CharacterTpl<T> {
    int m_baseDefense{};
    int m_baseAttack{};
    FirstAbility_t m_firstAbility{};
    SecondAbility_t m_secondAbility{};
    seneca::Dagger m_weapon{}; 

public:
    Rogue(const char* name, int healthMax, int baseAttack, int baseDefense)
        : CharacterTpl<T>(name, healthMax),
          m_baseDefense(baseDefense),
          m_baseAttack(baseAttack) {}
    
    // The following display function implementation was developed with guidance from ChatGPT.
    int getAttackAmnt() const override {
        double weaponDamage = static_cast<double>(m_weapon);
        return static_cast<int>(m_baseAttack + 2 * weaponDamage);
    }

    int getDefenseAmnt() const override {
        return m_baseDefense;
    }

    Character* clone() const override {
        return new Rogue(*this);
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    void attack(Character* enemy) override {
        std::cout << this->getName() << " is attacking " << enemy->getName() << "." << std::endl;

        m_firstAbility.useAbility(this);
        m_secondAbility.useAbility(this);

        int dmg = getAttackAmnt();

        m_firstAbility.transformDamageDealt(dmg);
        m_secondAbility.transformDamageDealt(dmg);

        std::cout << "    Rogue deals " << dmg << " melee damage!" << std::endl;

        enemy->takeDamage(dmg);
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    void takeDamage(int dmg) override {
        std::cout << this->getName() << " is attacked for " << dmg << " damage." << std::endl;
        std::cout << "    Rogue has a defense of " << m_baseDefense
                  << ". Reducing damage received." << std::endl;

        dmg -= m_baseDefense;
        dmg = std::max(0, dmg);

        m_firstAbility.transformDamageReceived(dmg);
        m_secondAbility.transformDamageReceived(dmg);

        CharacterTpl<T>::takeDamage(dmg);
    }
};

} // namespace seneca

#endif
