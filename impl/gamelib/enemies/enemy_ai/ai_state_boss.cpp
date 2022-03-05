
#include "ai_state_boss.hpp"

AiStateBoss::AiStateBoss()
{
    m_follow.setForgetRange(99999999999999999999.0f);
    m_shooter.setForgetRange(99999999999999999999.0f);
}

void AiStateBoss::update(float elapsed, EnemyBase* base)
{
    if (calculateDistanceToTarget() >= 73984) {
        return;
    }
    m_follow.setTarget(m_target);
    m_follow.setPosition(m_position);

    m_shooter.setTarget(m_target);
    m_shooter.setPosition(m_position);

    m_follow.update(elapsed, base);
    m_shooter.update(elapsed, base);
}
void AiStateBoss::setPathCalculator(WorldPathCalculatorInterface* pathCalculator)
{
    m_follow.setPathCalculator(pathCalculator);
}
void AiStateBoss::setProjectileSpawner(ProjectileSpawnerInterface* spawner)
{
    m_shooter.setProjectileSpawner(spawner);
}
