#include "spell_passive_dash.hpp"
#include "state_game.hpp"

SpellPassiveDash::SpellPassiveDash(CharacterSheetImgui& sheet)
    : SpellPassiveImpl(sheet)
{
}

void SpellPassiveDash::onEquip() { m_charSheet.setDashFactor("Spell", 0.8f); }

void SpellPassiveDash::onUnEquip() { m_charSheet.setDashFactor("Spell", 0.0f); }

std::string SpellPassiveDash::getName() const { return "Improve Dash"; }
