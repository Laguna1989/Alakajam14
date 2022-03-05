#include "enemy_crystal_large.hpp"
#include "enemies/enemy_ai/ai_state_follow_target.hpp"
#include "enemies/enemy_ai/ai_state_wait_for_target.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "state_game.hpp"

EnemyCrystalLarge::EnemyCrystalLarge(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : EnemyBase(world, def)
{
}

void EnemyCrystalLarge::doCreate()
{
    m_experience = 180;
    m_hitpoints = GP::EnemyCrystallLargeHitPoints();
    m_movementSpeed = 30.0f;
    m_closeCombatDamage = 80.0f;

    m_animation = std::make_shared<jt::Animation>();

    m_animation->add("assets/crystal_idle_large_green.png", "idle", jt::Vector2u { 48, 48 },
        { 0, 1, 2, 3, 4 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/crystal_shoot_large_green.png", "shoot", jt::Vector2u { 48, 48 },
        { 0, 1, 2, 3 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/crystal_dead_large_green.png", "dead", jt::Vector2u { 48, 48 },
        { 0, 1, 2, 3, 4, 5 }, 0.1f, getGame()->gfx().textureManager());

    m_animation->play("idle");
    //    m_animation->setScreenSizeHint(GP::GetScreenSize());

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
    waitState->setNextState("follow");
    waitState->setDetectRange(120.0f);
    getAiStateManager().registerState("wait", waitState);

    auto followState = std::make_shared<AiStateFollowTarget>();
    followState->setTarget(m_target);
    followState->setNextState("wait");
    followState->setForgetRange(200.0f);
    getAiStateManager().registerState("follow", followState);

    getAiStateManager().switchToState("wait");
}
