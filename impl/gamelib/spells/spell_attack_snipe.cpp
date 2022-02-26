#include "spell_attack_snipe.hpp"

SpellImpl::SpellImpl(StateGame& state)
    : m_state { state }
{
}
void SpellImpl::update(float elapsed) { m_cooldown -= elapsed; }

void SpellImpl::trigger()
{
    if (m_cooldown <= 0.0f) {
        doTrigger();
    }
}

SpellAttackSnipe::SpellAttackSnipe(StateGame& state)
    : SpellImpl { state }
{
}

void SpellAttackSnipe::doTrigger() { }
