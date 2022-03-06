#include "enemy_crystal_boss.hpp"
#include "enemies/enemy_ai/ai_state_boss.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "pathfinder/pathfinder.hpp"
#include "projectile_spawner_interface.hpp"
#include "state_game.hpp"

EnemyCrystalBoss::EnemyCrystalBoss(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def)
    : EnemyBase(world, def)
{
    m_experience = 5;
    m_hitpoints = GP::BossHitPoints();
    m_movementSpeed = GP::BossMovementSpeed();
    m_closeCombatDamage = 70.0f;
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
    //    m_animation->setScreenSizeHint(GP::GetScreenSize());

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

void EnemyCrystalBoss::doDie() { }
bool EnemyCrystalBoss::isBoss() { return true; }
