#ifndef ALAKAJAM14_AI_STATE_INTERFACE_HPP
#define ALAKAJAM14_AI_STATE_INTERFACE_HPP

#include "vector.hpp"
#include <memory>

class TargetInterface;
class Enemy;

class AiStateInterface {
public:
    virtual ~AiStateInterface() = default;
    virtual void update(float elapsed, Enemy* base) = 0;

    virtual void setTarget(std::weak_ptr<TargetInterface> target) = 0;
    virtual void setPosition(jt::Vector2f const& pos) = 0;
};

#endif // ALAKAJAM14_AI_STATE_INTERFACE_HPP
