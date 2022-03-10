#include "ai_state_boss_injured.hpp"
#include "enemies/enemy.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "projectile_spawner_interface.hpp"

AiStateBossInjured::AiStateBossInjured() { }

void AiStateBossInjured::update(float elapsed, Enemy* base)
{

    if (calculateDistanceToTarget() >= 73984) {
        return;
    }
    base->setPosition(jt::Vector2f { 2238, 2208 });
    handleShooting(elapsed);

    checkForStateSwitch(base);
}
void AiStateBossInjured::handleShooting(float elapsed)
{
    m_shootTimer -= elapsed;

    if (m_shootTimer <= 0) {
        m_shootTimer = 1.8f;
        jt::Vector2f const pos = m_position;

        auto const numberOfShots = 8;
        auto const angleIncrement = 360.0f / numberOfShots;
        auto const oddOffset = angleIncrement / 2.0f;
        for (auto i = 0U; i != numberOfShots; ++i) {
            float currentAngle = angleIncrement * i;
            float currentAngleOffset = m_oddShot ? oddOffset : 0.0f;

            float angle = jt::MathHelper::deg2rad(currentAngle + currentAngleOffset);
            jt::Vector2f velocity { sin(angle), cos(angle) };
            m_projectileSpawner->spawnCrystalProjectile(pos, velocity * GP::EnemyShotSpeed(), true);
        }
        m_oddShot = !m_oddShot;
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

void AiStateBossInjured::checkForStateSwitch(Enemy* base)
{
    float const currentHitpoints = base->getHitpoints();
    float const maxHitpoints = base->getInfo().hitpoints;

    if (currentHitpoints <= maxHitpoints * 0.33f) {
        base->getAiStateManager().switchToState(m_nextState);
    }
}
