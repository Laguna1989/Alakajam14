#ifndef ALAKAJAM14_AI_STATE_IMPL_HPP
#define ALAKAJAM14_AI_STATE_IMPL_HPP

#include "ai_state_interface.hpp"
#include <memory>

class TargetInterface;

class AiStateImpl : public AiStateInterface {
public:
    void setTarget(std::weak_ptr<TargetInterface> target) override;
    void setPosition(jt::Vector2f const& pos) override;

protected:
    std::weak_ptr<TargetInterface> m_target;
    jt::Vector2f m_position;

    float calculateDistanceToTarget() const;

private:
};

#endif // ALAKAJAM14_AI_STATE_IMPL_HPP
