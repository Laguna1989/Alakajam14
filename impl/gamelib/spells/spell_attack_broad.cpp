
#include "spell_attack_broad.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "state_game.hpp"

SpellAttackBroad::SpellAttackBroad(StateGame& state)
    : SpellTriggerImpl { state }
{
}

int SpellAttackBroad::getExperienceCost() const { return GP::SpellBroadExpCosts(); }

std::string SpellAttackBroad::getName() const { return "Broad Stroke"; }

void SpellAttackBroad::doTrigger()
{
    m_cooldown = 1.0f;
    auto const playerPosition = m_state.getPlayer()->getPosition();
    auto playerVelocity = m_state.getPlayer()->getVelocity();

    jt::MathHelper::normalizeMe(playerVelocity);

    // Shoot downwards if not moving.
    if (jt::MathHelper::length(playerVelocity) < 0.99f) {
        playerVelocity = { 0.0f, 1.0f };
    }

    for (float a = 0.0f; a <= 90.0f; a += 30.0f) {

        auto const projectileVelocity = jt::MathHelper::rotateBy(playerVelocity, a - 45.0f);
        jt::Vector2f const projectilePosition { playerPosition.x + projectileVelocity.x * 23.0f,
            playerPosition.y + projectileVelocity.y * 23.0f };
        m_state.spawnBroadProjectile(projectilePosition, projectileVelocity);
    }

    // TODO: Knockback.
}
