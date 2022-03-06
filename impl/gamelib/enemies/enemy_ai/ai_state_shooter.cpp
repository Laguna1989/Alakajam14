#include "ai_state_shooter.hpp"
#include "enemies/enemy_base.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "projectile_spawner_interface.hpp"
#include "target_interface.hpp"

void AiStateShooter::setNextState(std::string const& nextState) { m_nextState = nextState; }
void AiStateShooter::setForgetRange(float forgetRange) { m_forgetRange = forgetRange; }

void AiStateShooter::update(float elapsed, EnemyBase* base)
{
    if (m_projectileSpawner == nullptr) {
        return;
    }

    m_shootTimer -= elapsed;
    auto const playerPosition = m_target.lock()->getTargetPosition();
    auto const enemyPosition = m_position;

    auto diff = playerPosition - enemyPosition;

    auto const distanceSquared = jt::MathHelper::lengthSquared(diff);

    if (distanceSquared < m_forgetRange * m_forgetRange) {
        if (m_shootTimer <= 0) {
            jt::MathHelper::normalizeMe(diff);
            m_projectileSpawner->spawnCrystalProjectile(
                enemyPosition + diff * 10, diff * GP::EnemyShotSpeed(), false);
            m_shootTimer = GP::EnemyShotTimer();
        }
    } else {
        if (m_nextState != "") {
            base->getAiStateManager().switchToState(m_nextState);
        }
    }
}

void AiStateShooter::setProjectileSpawner(ProjectileSpawnerInterface* spawner)
{
    m_projectileSpawner = spawner;
}
