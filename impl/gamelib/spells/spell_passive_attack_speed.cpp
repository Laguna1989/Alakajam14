#include "spell_passive_attack_speed.hpp"
#include "state_game.hpp"

SpellPassiveAttackSpeed::SpellPassiveAttackSpeed(StateGame& state)
    : SpellPassiveImpl(state)
{
}

void SpellPassiveAttackSpeed::onEquip()
{
    m_state.getPlayer()->getCharSheet()->setAttackSpeedFactor("Spell", 0.15f);
}

void SpellPassiveAttackSpeed::onUnEquip()
{
    m_state.getPlayer()->getCharSheet()->setAttackSpeedFactor("Spell", 0.0f);
}

std::string SpellPassiveAttackSpeed::getName() const { return "Increased Attack Speed"; }
