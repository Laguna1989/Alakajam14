#include "spell_attack_snipe.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "player/player.hpp"
#include "state_game.hpp"

SpellAttackSnipe::SpellAttackSnipe(StateGame& state)
    : SpellTriggerImpl { state }
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

    auto const projectileVelocity = playerVelocity;

    jt::Vector2f const projectilePosition { playerPosition.x, playerPosition.y };
    m_state.spawnSnipeProjectile(projectilePosition,
        projectileVelocity * GP::SpellSnipeProjectileSpeed(),
        Damage { GP::SpellSnipeProjectileDamage()
            * m_state.getPlayer()->getCharSheet()->getMagicDamageFactor() });

    // TODO: Knockback.
}

int SpellAttackSnipe::getExperienceCost() const { return GP::SpellSnipeExpCosts(); }
std::string SpellAttackSnipe::getName() const { return "Mycelium Missile"; }
