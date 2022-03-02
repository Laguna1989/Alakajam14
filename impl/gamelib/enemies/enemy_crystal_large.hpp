#ifndef ALAKAJAM14_ENEMY_CRYSTAL_LARGE_HPP
#define ALAKAJAM14_ENEMY_CRYSTAL_LARGE_HPP

#include "enemy_base.hpp"
#include "pathfinder/node_interface.hpp"

class EnemyCrystalLarge : public EnemyBase {
public:
    EnemyCrystalLarge(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def);

private:
    void doCreate() override;
};

#endif // ALAKAJAM14_ENEMY_CRYSTAL_LARGE_HPP
