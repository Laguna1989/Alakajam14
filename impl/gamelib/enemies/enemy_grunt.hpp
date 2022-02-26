#ifndef ALAKAJAM14_ENEMY_GRUNT_HPP
#define ALAKAJAM14_ENEMY_GRUNT_HPP

#include "enemy_base.hpp"

class EnemyGrunt : public EnemyBase {
public:
    EnemyGrunt(
        std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);

    void doAI(float elapsed) override;

private:
    bool m_followingPlayer { false };
    void walkTowardsPlayer(jt::Vector2f diff);
};

#endif // ALAKAJAM14_ENEMY_GRUNT_HPP
