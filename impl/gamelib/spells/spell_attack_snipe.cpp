#include "spell_attack_snipe.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "snipe_projectile.hpp"
#include "state_game.hpp"

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

    // Shoot downwards if not moving.
    if (jt::MathHelper::length(playerVelocity) < 0.99f) {
        playerVelocity = { 0.0f, 1.0f };
    }

    auto projectileVelocity = playerVelocity;
    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(playerPosition.x + projectileVelocity.x * 16.0f,
        playerPosition.y + projectileVelocity.y * 16.0f);

    projectileVelocity = projectileVelocity * GP::SpellSnipeProjectileSpeed();

    auto projectile = std::make_shared<SnipeProjectile>(m_state.m_world, &bodyDef);
    projectile->setVelocity(projectileVelocity);
    m_state.spawnSnipeProjectile(projectile);

    // TODO: Knockback. This implementation is reverted automatically since we reset the player's
    // velocity each frame.
    // m_state.getPlayer()->addVelocity(projectileVelocity * -0.5f);
}

int SpellAttackSnipe::getExperienceCost() const { return 10; }
