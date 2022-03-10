#include "ai_state_boss_injured.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "projectile_spawner_interface.hpp"

#include <iostream>

AiStateBossInjured::AiStateBossInjured() { }

void AiStateBossInjured::update(float elapsed, Enemy* base)
{

    std::cout << "state injured\n";
    if (calculateDistanceToTarget() >= 73984) {
        return;
    }

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
void AiStateBossInjured::setPathCalculator(WorldPathCalculatorInterface* pathCalculator)
{
    //   m_follow.setPathCalculator(pathCalculator);
}
void AiStateBossInjured::setProjectileSpawner(ProjectileSpawnerInterface* spawner)
{
    m_projectileSpawner = spawner;
}
void AiStateBossInjured::setNextState(std::string const& nextState) { m_nextState = nextState; }
