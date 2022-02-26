#include "spell_impl.hpp"

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
