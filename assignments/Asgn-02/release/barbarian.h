/* Citation and Sources...
Assignment 02
Module: barbarian
Filename: barbarian.h
Author: Miguel Yap, StNo: 168049237, Email: myap1@myseneca.ca
-----------------------------------------------------------
- Line 39-43 was assisted with the help of ChatGPT.
- Line 54-66 was assisted with the help of ChatGPT.
- Line 69-79 was assisted with the help of ChatGPT.
-----------------------------------------------------------*/

#ifndef SENECA_BARBARIAN_H
#define SENECA_BARBARIAN_H

#include "characterTpl.h"
#include <iostream>
#include <algorithm> 

namespace seneca {

template <typename T, typename Ability_t, typename Weapon_t>
class Barbarian : public CharacterTpl<T> {
    int m_baseDefense{};
    int m_baseAttack{};
    Ability_t m_ability{};
    Weapon_t m_weapon[2]{}; 

public:
    Barbarian(const char* name, int healthMax, int baseAttack, int baseDefense,
              Weapon_t primaryWeapon, Weapon_t secondaryWeapon)
        : CharacterTpl<T>(name, healthMax),
          m_baseDefense(baseDefense),
          m_baseAttack(baseAttack) {
        m_weapon[0] = primaryWeapon;
        m_weapon[1] = secondaryWeapon;
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    int getAttackAmnt() const override {
        double w1 = static_cast<double>(m_weapon[0]);
        double w2 = static_cast<double>(m_weapon[1]);
        return static_cast<int>(m_baseAttack + (w1 / 2.0) + (w2 / 2.0));
    }

    int getDefenseAmnt() const override {
        return m_baseDefense;
    }

    Character* clone() const override {
        return new Barbarian(*this);
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    void attack(Character* enemy) override {
        std::cout << this->getName() << " is attacking " << enemy->getName() << "." << std::endl;

        m_ability.useAbility(this);

        int dmg = getAttackAmnt();

        m_ability.transformDamageDealt(dmg);

        std::cout << "    Barbarian deals " << dmg << " melee damage!" << std::endl;

        enemy->takeDamage(dmg);
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    void takeDamage(int dmg) override {
        std::cout << this->getName() << " is attacked for " << dmg << " damage." << std::endl;
        std::cout << "    Barbarian has a defense of " << m_baseDefense << ". Reducing damage received." << std::endl;

        dmg -= m_baseDefense;
        dmg = std::max(0, dmg);

        m_ability.transformDamageReceived(dmg);

        CharacterTpl<T>::takeDamage(dmg);
    }
};

} // namespace seneca

#endif
