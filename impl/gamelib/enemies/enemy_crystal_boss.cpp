#include "enemy_crystal_boss.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "pathfinder/pathfinder.hpp"
#include "random/random.hpp"
#include "state_game.hpp"

EnemyCrystalBoss::EnemyCrystalBoss(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state)
    : EnemyBase(world, def, state)
    , m_state(state)
{
    m_experience = 5000; // has no effect anyway -- said the dev ignoring glitch% speedrunners
    m_hitpoints = GP::BossHitPoints();
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
    circle.m_radius = GP::PlayerSize().x;

    fixtureDef.shape = &circle;
    fixtureDef.friction = 0.0f;
    getB2Body()->CreateFixture(&fixtureDef);
}

void EnemyCrystalBoss::doAI(float elapsed)
{
    // sry this is copypasted
    // Shoot
    m_shootTimer -= elapsed;
    auto const playerPosition = m_state.getPlayer()->getPosition();
    auto const enemyPosition = getPosition();

    auto diff = playerPosition - enemyPosition;

    auto const distanceSquared = jt::MathHelper::lengthSquared(diff);

    auto const shootRange = GP::EnemyShotRange();
    if (distanceSquared < shootRange * shootRange) {
        if (m_shootTimer <= 0) {
            int shots = m_hitpoints > GP::BossHitPoints() / 3 ? 8 : 12;
            for (int i = 0; i < shots; ++i) {
                auto aim = jt::Random::getRandomPointIn(jt::Rectf { -1.0f, -1.0f, 2.0f, 2.0f });
                jt::MathHelper::normalizeMe(aim);
                m_state.spawnCrystalProjectile(
                    enemyPosition + aim * 10, aim * GP::EnemyShotSpeed());
            }
            m_shootTimer = GP::EnemyShotTimer();
        }
    }

    // Move
    m_timeToPathfind -= elapsed;
    if (m_followingPlayer) {
        auto const forgetRange = 200;
        if (distanceSquared > forgetRange * forgetRange) {
            m_followingPlayer = false;
            return;
        }

        walkTowardsPlayer(diff);

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
                if (distanceSquared < 24 * 24) {
                    m_state.getPlayer()->receiveDamage(Damage { 80.0f });
                    m_attackCooldown = 1.0f;
                }
                m_animation->play("idle");
            }
        }

    } else {
        int detectRange = 100;
        if (distanceSquared < detectRange * detectRange) {
            m_followingPlayer = true;
            // TODO render exclamation mark
        }
        // idle
    }
}
void EnemyCrystalBoss::walkTowardsPlayer(jt::Vector2f diff)
{
    jt::MathHelper::normalizeMe(diff);

    if (m_timeToPathfind <= 0) {
        auto const tileForEnemy = m_state.getTileAtPosition(getPosition());
        auto const tileForPlayer = m_state.getTileAtPosition(m_state.getPlayer()->getPosition());
        m_cachedPath = jt::pathfinder::calculatePath(tileForEnemy, tileForPlayer);
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

    setVelocity(diffToTile * 15.0f);
}
