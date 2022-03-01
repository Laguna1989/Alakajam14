#ifndef ALAKAJAM14_ENEMY_CRYSTAL_LARGE_HPP
#define ALAKAJAM14_ENEMY_CRYSTAL_LARGE_HPP

#include "enemy_base.hpp"
#include "pathfinder/node_interface.hpp"

class EnemyCrystalLarge : public EnemyBase {
public:
    EnemyCrystalLarge(
        std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);

private:
    void doAI(float elapsed) override;

private:
    void doCreate() override;
    bool m_followingPlayer { false };
    float m_timeSinceTriggeredAttack { 0.0f };
    float m_timeToPathfind { 0.0f };
    void walkTowardsPlayer();
    std::vector<std::shared_ptr<jt::pathfinder::NodeInterface>> m_cachedPath;
};

#endif // ALAKAJAM14_ENEMY_CRYSTAL_LARGE_HPP
