#include "enemy_grunt.hpp"
#include "math_helper.hpp"
#include "pathfinder/pathfinder.hpp"
#include "state_game.hpp"

EnemyGrunt::EnemyGrunt(
    std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state)
    : EnemyBase { world, def, state }
{
}

void EnemyGrunt::doAI(float elapsed)
{

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
void EnemyGrunt::walkTowardsPlayer(jt::Vector2f diff)
{
    jt::MathHelper::normalizeMe(diff);

    auto const tileForEnemy = m_state.getTileAtPosition(getPosition());
    auto const tileForPlayer = m_state.getTileAtPosition(m_state.getPlayer()->getPosition());

    auto path = jt::pathfinder::calculatePath(tileForEnemy, tileForPlayer);
    std::cout << path.size() << std::endl;
    if (path.size() < 2) {
        return;
    }

    auto const nextTilePosition = path.at(1)->getTilePosition();

    std::cout << "move to " << nextTilePosition.x << ", " << nextTilePosition.y << std::endl;

    // TODO move to GP
    jt::Vector2f const nextTilePositionFloat { nextTilePosition.x * 16.0f,
        nextTilePosition.y * 16.0f };

    auto const enemyPosition = getPosition();
    auto diffToTile = nextTilePositionFloat - enemyPosition;
    jt::MathHelper::normalizeMe(diffToTile);

    setVelocity(diffToTile * 10.0f);
}
