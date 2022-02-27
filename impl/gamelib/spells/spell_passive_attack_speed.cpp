#include "spell_passive_attack_speed.hpp"
#include "state_game.hpp"

SpellPassiveAttackSpeed::SpellPassiveAttackSpeed(CharacterSheetImgui& sheet)
    : SpellPassiveImpl(sheet)
{
}

void SpellPassiveAttackSpeed::onEquip() { m_charSheet.setAttackSpeedFactor("Spell", 0.175); }

void SpellPassiveAttackSpeed::onUnEquip() { m_charSheet.setAttackSpeedFactor("Spell", 0.0f); }

std::string SpellPassiveAttackSpeed::getName() const { return "Increased Attack Speed"; }
