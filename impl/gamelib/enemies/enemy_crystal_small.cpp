#include "enemy_crystal_small.hpp"
#include "enemy_ai/ai_state_follow_target.hpp"
#include "enemy_ai/ai_state_wait_for_target.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "state_game.hpp"

EnemyCrystalSmall::EnemyCrystalSmall(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : EnemyBase(world, def)
{
}

void EnemyCrystalSmall::doCreate()
{
    m_experience = 7;
    m_hitpoints = GP::EnemyCrystallSmallHitPoints();
    m_movementSpeed = 45.0f;
    m_closeCombatDamage = 30.0f;

    m_animation = std::make_shared<jt::Animation>();

    m_animation->add("assets/crystal_idle_small_green.png", "idle", jt::Vector2u { 16, 16 },
        { 0, 1, 2, 3, 4 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/crystal_shoot_small_green.png", "shoot", jt::Vector2u { 16, 16 },
        { 0, 1, 2, 3 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->add("assets/crystal_dead_small_green.png", "dead", jt::Vector2u { 48, 48 },
        { 0, 1, 2, 3 }, 0.1f, getGame()->gfx().textureManager());
    m_animation->play("idle");
    m_animation->setScreenSizeHint(GP::GetScreenSize());

    b2FixtureDef fixtureDef;
    b2CircleShape circle {};
    circle.m_radius = GP::PlayerSize().x / 2.0f - 4.0f;

    fixtureDef.shape = &circle;
    fixtureDef.friction = 0.0f;
    getB2Body()->CreateFixture(&fixtureDef);

    auto waitState = std::make_shared<AiStateWaitForTarget>();
    waitState->setTarget(m_target);
    waitState->setNextState("follow");
    getAiStateManager().registerState("wait", waitState);

    auto followState = std::make_shared<AiStateFollowTarget>();
    followState->setTarget(m_target);
    followState->setNextState("wait");
    followState->setPathCalculator(m_pathCalculator);
    getAiStateManager().registerState("follow", followState);

    getAiStateManager().switchToState("wait");
}
