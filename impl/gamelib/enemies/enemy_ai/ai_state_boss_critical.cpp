#include "ai_state_boss_critical.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "projectile_spawner_interface.hpp"

AiStateBossCritical::AiStateBossCritical() { }

void AiStateBossCritical::update(float elapsed, Enemy* base)
{
    if (calculateDistanceToTarget() >= 73984) {
        return;
    }

    m_shootTimer -= elapsed;

    if (m_shootTimer <= 0) {
        m_shootTimer = 0.4f;
        jt::Vector2f const pos = m_position;
        m_currentAngle += 20.0f;
        float angle = jt::MathHelper::deg2rad(m_currentAngle);
        jt::Vector2f velocity { sin(angle), cos(angle) };
        m_projectileSpawner->spawnCrystalProjectile(pos, velocity * GP::EnemyShotSpeed(), true);
    }
}

void AiStateBossCritical::setPathCalculator(WorldPathCalculatorInterface* pathCalculator)
{
    //   m_follow.setPathCalculator(pathCalculator);
}

void AiStateBossCritical::setProjectileSpawner(ProjectileSpawnerInterface* spawner)
{
    m_projectileSpawner = spawner;
}

void AiStateBossCritical::setNextState(std::string const& nextState) { m_nextState = nextState; }
