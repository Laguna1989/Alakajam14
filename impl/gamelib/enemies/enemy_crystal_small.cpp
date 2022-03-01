#include "enemy_crystal_small.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "pathfinder/pathfinder.hpp"
#include "random/random.hpp"
#include "state_game.hpp"

EnemyCrystalSmall::EnemyCrystalSmall(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state)
    : EnemyBase(world, def, state)
{
    m_experience = 7;
    m_hitpoints = GP::EnemyCrystallSmallHitPoints();
}

void EnemyCrystalSmall::doCreate()
{
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
}

void EnemyCrystalSmall::doPerformAI(float elapsed)
{
    m_timeToPathfind -= elapsed;
    auto const targetPosition = m_target.lock()->getTargetPosition();
    auto const enemyPosition = getPosition();

    auto diff = targetPosition - enemyPosition;

    auto const distanceSquared = jt::MathHelper::lengthSquared(diff);

    if (m_followingTarget) {
        auto const forgetRange = 180;
        if (distanceSquared > forgetRange * forgetRange) {
            m_followingTarget = false;
            return;
        }

        walkTowardsTarget();

        if (m_timeSinceTriggeredAttack < 0) {
            if (distanceSquared < 18 * 18) {
                if (canAttack()) {
                    m_animation->play("shoot");

                    m_timeSinceTriggeredAttack = m_animation->getCurrentAnimTotalTime();
                }
            }
        } else {
            m_timeSinceTriggeredAttack -= elapsed;
            setVelocity(jt::Vector2f { 0.0f, 0.0f });
            if (m_timeSinceTriggeredAttack <= 0) {
                // TODO Visual candy
                if (distanceSquared < 22 * 22) {
                    m_target.lock()->applyDamageToTarget(Damage { 30.0f });
                    m_attackCooldown = 1.0f;
                }
                m_animation->play("idle");
            }
        }

    } else {
        int detectRange = 100;
        if (distanceSquared < detectRange * detectRange) {
            m_followingTarget = true;
            // TODO render exclamation mark
        }
        // idle
    }
}

void EnemyCrystalSmall::walkTowardsTarget()
{
    if (m_pathCalculator == nullptr) {
        return;
    }
    if (m_timeToPathfind <= 0) {
        m_cachedPath
            = m_pathCalculator->calculatePath(getPosition(), m_target.lock()->getTargetPosition());
        m_timeToPathfind = jt::Random::getFloat(0.4f, 0.5f);
    }

    if (m_cachedPath.size() < 2) {
        return;
    }

    auto const& nextTilePosition = m_cachedPath.at(1)->getTilePosition();

    // TODO move to GP
    jt::Vector2f const nextTilePositionFloat { nextTilePosition.x * 16.0f + 8,
        nextTilePosition.y * 16.0f + 8 };

    auto const enemyPosition = getPosition();
    auto diffToTile = nextTilePositionFloat - enemyPosition;
    jt::MathHelper::normalizeMe(diffToTile);

    setVelocity(diffToTile * 45.0f);
}
