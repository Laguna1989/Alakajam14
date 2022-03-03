#include "enemy_crystal_boss.hpp"
#include "enemies/enemy_ai/ai_state_boss.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "pathfinder/pathfinder.hpp"
#include "projectile_spawner_interface.hpp"
#include "random/random.hpp"
#include "state_game.hpp"
#include "state_menu.hpp"

EnemyCrystalBoss::EnemyCrystalBoss(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : EnemyBase(world, def)
{
    m_experience = 5; // has no effect anyway -- said the dev ignoring glitch% speedrunners
    m_hitpoints = GP::BossHitPoints();
    m_movementSpeed = GP::BossMovementSpeed();
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
    m_animation->setScreenSizeHint(GP::GetScreenSize());

    b2FixtureDef fixtureDef;
    b2CircleShape circle {};
    circle.m_radius = GP::PlayerSize().x * 1.5f;

    fixtureDef.shape = &circle;
    fixtureDef.filter.categoryBits = GP::PhysicsCollisionCategoryEnemies();
    fixtureDef.filter.maskBits = GP::PhysicsCollisionCategoryWalls()
        | GP::PhysicsCollisionCategoryPlayer() | GP::PhysicsCollisionCategoryPlayerShots();

    getB2Body()->CreateFixture(&fixtureDef);

    auto bossState = std::make_shared<AiStateBoss>();
    bossState->setTarget(m_target);
    bossState->setPathCalculator(m_pathCalculator);
    bossState->setProjectileSpawner(m_projectileSpawner);
    getAiStateManager().registerState("boss", bossState);

    getAiStateManager().switchToState("boss");
}

void EnemyCrystalBoss::doDie()
{
    getGame()->getStateManager().switchState(std::make_shared<StateMenu>());
}
