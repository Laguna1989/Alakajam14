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
    m_experience = 50;
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

    b2FixtureDef fixtureDef;
    b2CircleShape circle {};
    circle.m_radius = GP::PlayerSize().x / 2.0f;

    fixtureDef.shape = &circle;
    fixtureDef.friction = 0.0f;
    getB2Body()->CreateFixture(&fixtureDef);
}

void EnemyCrystalSmall::doAI(float elapsed)
{
    m_timeToPathfind -= elapsed;
    auto const playerPosition = m_state.getPlayer()->getPosition();
    auto const enemyPosition = getPosition();

    auto diff = playerPosition - enemyPosition;

    auto const distance = jt::MathHelper::length(diff);

    if (m_followingPlayer) {
        auto const forgetRange = 200;
        if (distance > forgetRange) {
            m_followingPlayer = false;
            return;
        }

        walkTowardsPlayer(diff);

    } else {
        int detectRange = 100;
        if (distance < detectRange) {
            m_followingPlayer = true;
            // TODO render exclamation mark
        }
        // idle
    }
}

void EnemyCrystalSmall::walkTowardsPlayer(jt::Vector2f diff)
{
    jt::MathHelper::normalizeMe(diff);

    if (m_timeToPathfind <= 0) {
        auto const tileForEnemy = m_state.getTileAtPosition(getPosition());
        auto const tileForPlayer = m_state.getTileAtPosition(m_state.getPlayer()->getPosition());
        m_path = jt::pathfinder::calculatePath(tileForEnemy, tileForPlayer);
        m_timeToPathfind = jt::Random::getFloat(0.4f, 0.5f);
    }

    if (m_path.size() < 2) {
        return;
    }

    auto const& nextTilePosition = m_path.at(1)->getTilePosition();

    // TODO move to GP
    jt::Vector2f const nextTilePositionFloat { nextTilePosition.x * 16.0f + 8,
        nextTilePosition.y * 16.0f + 8 };

    auto const enemyPosition = getPosition();
    auto diffToTile = nextTilePositionFloat - enemyPosition;
    jt::MathHelper::normalizeMe(diffToTile);

    setVelocity(diffToTile * 20.0f);
}
