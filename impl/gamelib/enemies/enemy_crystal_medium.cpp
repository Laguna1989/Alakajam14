#include "enemy_crystal_medium.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "state_game.hpp"

EnemyCrystalMedium::EnemyCrystalMedium(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state)
    : EnemyBase(world, def, state)
{
    m_experience = 50;
    m_hitpoints = GP::EnemyCrystallMediumHitPoints();
}

void EnemyCrystalMedium::doPerformAI(float elapsed)
{
    m_shootTimer -= elapsed;
    auto const playerPosition = m_state.getPlayer()->getPosition();
    auto const enemyPosition = getPosition();

    auto diff = playerPosition - enemyPosition;

    auto const distanceSquared = jt::MathHelper::lengthSquared(diff);

    auto const shootRange = GP::EnemyShotRange();
    if (distanceSquared < shootRange * shootRange) {
        if (m_shootTimer <= 0) {
            jt::MathHelper::normalizeMe(diff);
            m_state.spawnCrystalProjectile(enemyPosition + diff * 10, diff * GP::EnemyShotSpeed());
            m_shootTimer = GP::EnemyShotTimer();
        }
    }
}

void EnemyCrystalMedium::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();

    m_animation->add("assets/crystal_idle_mid_green.png", "idle", jt::Vector2u { 16, 16 },
        { 0, 1, 2, 3, 4 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/crystal_shoot_mid_green.png", "shoot", jt::Vector2u { 16, 16 },
        { 0, 1, 2, 3 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/crystal_dead_mid_green.png", "dead", jt::Vector2u { 48, 48 },
        { 0, 1, 2, 3, 4, 5 }, 0.1f, getGame()->gfx().textureManager());

    m_animation->play("idle");
    m_animation->setScreenSizeHint(GP::GetScreenSize());

    b2FixtureDef fixtureDef;
    b2CircleShape circle {};
    circle.m_radius = GP::PlayerSize().x / 2.0f;

    fixtureDef.shape = &circle;
    fixtureDef.friction = 0.0f;
    getB2Body()->CreateFixture(&fixtureDef);
}
