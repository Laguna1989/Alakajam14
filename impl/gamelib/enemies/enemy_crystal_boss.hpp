#ifndef ALAKAJAM14_ENEMY_CRYSTAL_BOSS_HPP
#define ALAKAJAM14_ENEMY_CRYSTAL_BOSS_HPP

#include "enemy_base.hpp"

class EnemyCrystalBoss : public EnemyBase {
public:
    EnemyCrystalBoss(
        std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);

private:
    void doAI(float elapsed) override;

private:
    void doCreate() override;
};

#endif // ALAKAJAM14_ENEMY_CRYSTAL_BOSS_HPP
