#ifndef ALAKAJAM14_ENEMY_CRYSTAL_BOSS_HPP
#define ALAKAJAM14_ENEMY_CRYSTAL_BOSS_HPP

#include "enemy_base.hpp"
#include "pathfinder/node_interface.hpp"

class EnemyCrystalBoss : public EnemyBase {
public:
    EnemyCrystalBoss(
        std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);

private:
    void doPerformAI(float elapsed) override;
    void doDie() override;
    void doCreate() override;
    float m_shootTimer { -1.0f };
    StateGame& m_state;
    void walkTowardsPlayer(jt::Vector2f diff);
    bool m_followingPlayer { false };
    float m_timeSinceTriggeredAttack { 0.0f };
    float m_timeToPathfind { 0.0f };
    std::vector<std::shared_ptr<jt::pathfinder::NodeInterface>> m_cachedPath;
};

#endif // ALAKAJAM14_ENEMY_CRYSTAL_BOSS_HPP
