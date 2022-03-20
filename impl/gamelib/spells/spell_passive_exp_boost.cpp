#include "spell_passive_exp_boost.hpp"

SpellPassiveExpBoost::SpellPassiveExpBoost(CharacterSheetImgui& sheet)
    : SpellPassiveImpl(sheet)
{
}

void SpellPassiveExpBoost::onEquip() { m_charSheet.setExpBoostValue("spell", 2); }
void SpellPassiveExpBoost::onUnEquip() { m_charSheet.setExpBoostValue("spell", 0); }

std::string SpellPassiveExpBoost::getName() const { return "Passive: Improved Experience Gain"; }
