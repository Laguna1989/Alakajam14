#ifndef ALAKAJAM14_ENEMY_CRYSTAL_SMALL_HPP
#define ALAKAJAM14_ENEMY_CRYSTAL_SMALL_HPP

#include "enemy_base.hpp"
#include "pathfinder/node_interface.hpp"

class EnemyCrystalSmall : public EnemyBase {
public:
    EnemyCrystalSmall(
        std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);

private:
    bool m_followingPlayer { false };
    float m_timeToPathfind { -1.0f };
    float m_timeSinceTriggeredAttack { -1.0f };
    std::vector<std::shared_ptr<jt::pathfinder::NodeInterface>> m_cachedPath;

    void doAI(float elapsed) override;
    void doCreate() override;
    void walkTowardsPlayer(jt::Vector2f diff);
};

#endif // ALAKAJAM14_ENEMY_CRYSTAL_SMALL_HPP
