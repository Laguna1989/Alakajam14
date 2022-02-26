#include "spell_attack_snipe.hpp"
#include "math_helper.hpp"
#include "snipe_projectile.hpp"
#include "state_game.hpp"

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

void SpellAttackSnipe::doTrigger()
{
    m_cooldown = 1.0f;
    auto const playerPosition = m_state.getPlayer()->getPosition();
    auto playerVelocity = m_state.getPlayer()->getVelocity();

    jt::MathHelper::normalizeMe(playerVelocity);

    auto projectileVelocity = playerVelocity;
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(playerPosition.x + projectileVelocity.x * 16.0f,
        playerPosition.y + projectileVelocity.y * 16.0f);

    projectileVelocity = projectileVelocity * 100.0f;

    auto projectile = std::make_shared<SnipeProjectile>(m_state.m_world, &bodyDef);
    projectile->setVelocity(projectileVelocity);
    m_state.spawnSnipeProjectile(projectile);
}

int SpellAttackSnipe::getExperienceCost() const { return 10; }
