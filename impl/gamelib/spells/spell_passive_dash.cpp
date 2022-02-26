#include "spell_passive_dash.hpp"
#include "state_game.hpp"

SpellPassiveDash::SpellPassiveDash(StateGame& state)
    : SpellPassiveImpl(state)
{
}

void SpellPassiveDash::onEquip()
{
    m_state.getPlayer()->getCharSheet()->setDashFactor("Spell", 0.8f);
}

void SpellPassiveDash::onUnEquip()
{
    m_state.getPlayer()->getCharSheet()->setDashFactor("Spell", 0.0f);
}

std::string SpellPassiveDash::getName() const { return std::string(); }
