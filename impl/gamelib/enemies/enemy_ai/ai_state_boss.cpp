#include "ai_state_boss.hpp"
#include "enemies/enemy.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "projectile_spawner_interface.hpp"

AiStateBoss::AiStateBoss() { }

void AiStateBoss::update(float elapsed, Enemy* base)
{
    if (calculateDistanceToTarget() >= 73984) {
        return;
    }

    handleShooting(elapsed);

    checkForStateSwitch(base);
}

void AiStateBoss::checkForStateSwitch(Enemy* base)
{
    float const currentHitpoints = base->getHitpoints();
    float const maxHitpoints = base->getInfo().hitpoints;

    if (currentHitpoints <= maxHitpoints * 0.66f) {
        base->getAiStateManager().switchToState(m_nextState);
    }
}

void AiStateBoss::handleShooting(float elapsed)
{
    m_shootTimer -= elapsed;

    if (m_shootTimer <= 0) {
        m_shootTimer = 2.0f;
        jt::Vector2f const pos = m_position;

        auto const numberOfShots = 8;
        auto const angleIncrement = 360.0f / numberOfShots;
        for (auto i = 0U; i != numberOfShots; ++i) {
            float angle = jt::MathHelper::deg2rad(angleIncrement * i);
            jt::Vector2f velocity { sin(angle), cos(angle) };
            m_projectileSpawner->spawnCrystalProjectile(pos, velocity * GP::EnemyShotSpeed(), true);
        }
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
void AiStateBoss::setNextState(std::string const& nextState) { m_nextState = nextState; }
