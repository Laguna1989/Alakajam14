#include "enemy_crystal_medium.hpp"
#include "enemies/enemy_ai/ai_state_shooter.hpp"
#include "enemies/enemy_ai/ai_state_wait_for_target.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "projectile_spawner_interface.hpp"
#include "state_game.hpp"

EnemyCrystalMedium::EnemyCrystalMedium(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : EnemyBase(world, def)
{
}

void EnemyCrystalMedium::doCreate()
{
    m_experience = 40;
    m_hitpoints = GP::EnemyCrystallMediumHitPoints();
    m_closeCombatDamage = 0.0f;

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
    fixtureDef.filter.categoryBits = GP::PhysicsCollisionCategoryEnemies();
    fixtureDef.filter.maskBits = GP::PhysicsCollisionCategoryWalls()
        | GP::PhysicsCollisionCategoryPlayer() | GP::PhysicsCollisionCategoryPlayerShots();

    getB2Body()->CreateFixture(&fixtureDef);

    auto waitState = std::make_shared<AiStateWaitForTarget>();
    waitState->setTarget(m_target);
    waitState->setNextState("shoot");
    waitState->setDetectRange(GP::EnemyShotRange());
    getAiStateManager().registerState("wait", waitState);

    auto shooterState = std::make_shared<AiStateShooter>();
    shooterState->setTarget(m_target);
    shooterState->setNextState("wait");
    shooterState->setForgetRange(GP::EnemyShotRange());
    shooterState->setProjectileSpawner(m_projectileSpawner);
    getAiStateManager().registerState("shoot", shooterState);

    getAiStateManager().switchToState("wait");
}
