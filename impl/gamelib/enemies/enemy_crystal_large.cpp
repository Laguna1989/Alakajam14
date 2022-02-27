#include "enemy_crystal_large.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"

EnemyCrystalLarge::EnemyCrystalLarge(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state)
    : EnemyBase(world, def, state)
{
    m_experience = 50;
    m_hitpoints = GP::EnemyCrystallLargeHitPoints();
}

void EnemyCrystalLarge::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();

    m_animation->add("assets/crystal_idle_large_green.png", "idle", jt::Vector2u { 48, 48 },
        { 0, 1, 2, 3, 4 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/crystal_shoot_large_green.png", "shoot", jt::Vector2u { 48, 48 },
        { 0, 1, 2, 3 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/crystal_dead_large_green.png", "dead", jt::Vector2u { 48, 48 },
        { 0, 1, 2, 3, 4, 5 }, 0.1f, getGame()->gfx().textureManager());

    m_animation->play("idle");

    b2FixtureDef fixtureDef;
    b2CircleShape circle {};
    circle.m_radius = GP::PlayerSize().x / 2.0f;

    fixtureDef.shape = &circle;
    fixtureDef.friction = 0.0f;
    getB2Body()->CreateFixture(&fixtureDef);
}

void EnemyCrystalLarge::doAI(float elapsed) { }
