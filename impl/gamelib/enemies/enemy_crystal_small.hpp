#ifndef ALAKAJAM14_ENEMY_CRYSTAL_SMALL_HPP
#define ALAKAJAM14_ENEMY_CRYSTAL_SMALL_HPP

#include "enemy_base.hpp"

class EnemyCrystalSmall : public EnemyBase {
public:
    EnemyCrystalSmall(
        std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);

private:
    void doAI(float elapsed) override;
    void doCreate() override;
};

#endif // ALAKAJAM14_ENEMY_CRYSTAL_SMALL_HPP
