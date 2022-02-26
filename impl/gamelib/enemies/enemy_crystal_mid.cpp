#include "enemy_crystal_mid.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"

EnemyCrystalMid::EnemyCrystalMid(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state)
    : EnemyBase(world, def, state)
{
    m_experience = 50;
}

void EnemyCrystalMid::doAI(float elapsed) { }

void EnemyCrystalMid::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();

    m_animation->add("assets/crystal_idle_mid_green.png", "idle", jt::Vector2u { 16, 16 },
        { 0, 1, 2, 3, 4 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/crystal_shoot_mid_green.png", "idle", jt::Vector2u { 16, 16 },
        { 0, 1, 2, 3 }, 0.1f, getGame()->gfx().textureManager());

    m_animation->play("idle");

    b2FixtureDef fixtureDef;
    b2CircleShape circle {};
    circle.m_radius = GP::PlayerSize().x / 2.0f;

    fixtureDef.shape = &circle;
    fixtureDef.friction = 0.0f;
    getB2Body()->CreateFixture(&fixtureDef);
}
