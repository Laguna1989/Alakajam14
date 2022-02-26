#include "enemy_crystal_small.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"

EnemyCrystalSmall::EnemyCrystalSmall(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state)
    : EnemyBase(world, def, state)
{
    m_experience = 50;
}

void EnemyCrystalSmall::doAI(float elapsed) { }

void EnemyCrystalSmall::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();

    m_animation->add("assets/crystal_idle_small_green.png", "idle", jt::Vector2u { 16, 16 },
        { 0, 1, 2, 3, 4 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/crystal_shoot_small_green.png", "idle", jt::Vector2u { 16, 16 },
        { 0, 1, 2, 3 }, 0.1f, getGame()->gfx().textureManager());

    m_animation->play("idle");

    b2FixtureDef fixtureDef;
    b2CircleShape circle {};
    circle.m_radius = GP::PlayerSize().x / 2.0f;

    fixtureDef.shape = &circle;
    fixtureDef.friction = 0.0f;
    getB2Body()->CreateFixture(&fixtureDef);
}
