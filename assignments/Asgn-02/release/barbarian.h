/* Citation and Sources...
Assignment 02
Module: barbarian
Filename: barbarian.h
Author: Miguel Yap, StNo: 168049237, Email: myap1@myseneca.ca
-----------------------------------------------------------
- Line 37-40 was assisted with the help of ChatGPT.
- Line 53-65 was assisted with the help of ChatGPT.
- Line 69-80 was assisted with the help of ChatGPT.
-----------------------------------------------------------*/

#ifndef SENECA_BARBARIAN_H
#define SENECA_BARBARIAN_H

#include "characterTpl.h"
#include <iostream>
#include <algorithm> // for std::max

namespace seneca {

template <typename T, typename Ability_t, typename Weapon_t>
class Barbarian : public CharacterTpl<T> {
    int m_baseDefense{};
    int m_baseAttack{};
    Ability_t m_ability{};
    Weapon_t m_weapon[2]{}; // primary + secondary

public:
    // Constructor
    Barbarian(const char* name, int healthMax, int baseAttack, int baseDefense,
              Weapon_t primaryWeapon, Weapon_t secondaryWeapon)
        : CharacterTpl<T>(name, healthMax),
          m_baseDefense(baseDefense),
          m_baseAttack(baseAttack) {
        m_weapon[0] = primaryWeapon;
        m_weapon[1] = secondaryWeapon;
    }

    // Attack power formula
    int getAttackAmnt() const override {
        // average both weapons, then add base attack
        double w1 = static_cast<double>(m_weapon[0]);
        double w2 = static_cast<double>(m_weapon[1]);
        return static_cast<int>(m_baseAttack + (w1 / 2.0) + (w2 / 2.0));
    }

    // Defense
    int getDefenseAmnt() const override {
        return m_baseDefense;
    }

    // Clone
    Character* clone() const override {
        return new Barbarian(*this);
    }

    // Attack action
    void attack(Character* enemy) override {
        std::cout << this->getName() << " is attacking " << enemy->getName() << "." << std::endl;

        // activate ability on self
        m_ability.useAbility(this);

        // get base attack
        int dmg = getAttackAmnt();

        // ability may enhance it
        m_ability.transformDamageDealt(dmg);

        std::cout << "    Barbarian deals " << dmg << " melee damage!" << std::endl;

        // apply damage to enemy
        enemy->takeDamage(dmg);
    }

    // Take damage
    void takeDamage(int dmg) override {
        std::cout << this->getName() << " is attacked for " << dmg << " damage." << std::endl;
        std::cout << "    Barbarian has a defense of " << m_baseDefense << ". Reducing damage received." << std::endl;

        // reduce by defense
        dmg -= m_baseDefense;
        dmg = std::max(0, dmg);

        // ability may further modify it
        m_ability.transformDamageReceived(dmg);

        // update health via base class
        CharacterTpl<T>::takeDamage(dmg);
    }
};

} // namespace seneca

#endif
