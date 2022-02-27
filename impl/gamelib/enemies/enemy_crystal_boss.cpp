#include "enemy_crystal_boss.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"

EnemyCrystalBoss::EnemyCrystalBoss(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state)
    : EnemyBase(world, def, state)
{
}

void EnemyCrystalBoss::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();

    m_animation->add("assets/pinky_idle.png", "idle", jt::Vector2u { 128, 128 },
        { 0, 1, 2, 3, 4, 5 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/pinky_shoot.png", "shoot", jt::Vector2u { 128, 128 }, { 0, 1, 2, 3 },
        0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/pinky_dead.png", "dead", jt::Vector2u { 128, 128 },
        { 0, 1, 2, 3, 4, 5, 6 }, 0.1f, getGame()->gfx().textureManager());

    m_animation->play("idle");

    b2FixtureDef fixtureDef;
    b2CircleShape circle {};
    circle.m_radius = GP::PlayerSize().x / 2.0f;

    fixtureDef.shape = &circle;
    fixtureDef.friction = 0.0f;
    getB2Body()->CreateFixture(&fixtureDef);
}

void EnemyCrystalBoss::doAI(float elapsed) { }
