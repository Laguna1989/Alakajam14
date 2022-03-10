
#include "ai_state_boss.hpp"

AiStateBoss::AiStateBoss() { }

void AiStateBoss::update(float elapsed, Enemy* base)
{
    if (calculateDistanceToTarget() >= 73984) {
        return;
    }
}
void AiStateBoss::setPathCalculator(WorldPathCalculatorInterface* pathCalculator)
{
    //   m_follow.setPathCalculator(pathCalculator);
}
void AiStateBoss::setProjectileSpawner(ProjectileSpawnerInterface* spawner)
{
    m_projectileSpawner = spawner;
}
