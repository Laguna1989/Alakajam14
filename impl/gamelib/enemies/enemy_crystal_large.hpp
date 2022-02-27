#ifndef ALAKAJAM14_ENEMY_CRYSTAL_LARGE_HPP
#define ALAKAJAM14_ENEMY_CRYSTAL_LARGE_HPP

#include "box2dwrapper/box2d_world_interface.hpp"
#include "enemy_base.hpp"
#include <memory>

class EnemyCrystalLarge : public EnemyBase {
public:
    EnemyCrystalLarge(
        std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, StateGame& state);

private:
    void doAI(float elapsed) override;

private:
    void doCreate() override;
};

#endif // ALAKAJAM14_ENEMY_CRYSTAL_LARGE_HPP
