#ifndef ALAKAJAM14_EXPERIENCE_SPAWNER_INTERFACE_HPP
#define ALAKAJAM14_EXPERIENCE_SPAWNER_INTERFACE_HPP

#include "vector.hpp"

class ExperienceSpawnerInterface {
public:
    virtual ~ExperienceSpawnerInterface() = default;
    virtual void spawnExperience(int amount, jt::Vector2f const& pos, bool single) = 0;
};

#endif // ALAKAJAM14_EXPERIENCE_SPAWNER_INTERFACE_HPP
