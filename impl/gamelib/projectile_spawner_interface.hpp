#ifndef ALAKAJAM14_PROJECTILE_SPAWNER_INTERFACE_HPP
#define ALAKAJAM14_PROJECTILE_SPAWNER_INTERFACE_HPP

#include "vector.hpp"

class ProjectileSpawnerInterface {
public:
    virtual ~ProjectileSpawnerInterface() = default;
    virtual void spawnCrystalProjectile(
        jt::Vector2f const& position, jt::Vector2f const& velocity, bool isBoss)
        = 0;
};

#endif // ALAKAJAM14_PROJECTILE_SPAWNER_INTERFACE_HPP
