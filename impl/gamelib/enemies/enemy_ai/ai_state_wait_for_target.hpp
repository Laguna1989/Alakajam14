#ifndef ALAKAJAM14_AI_STATE_WAIT_FOR_TARGET_HPP
#define ALAKAJAM14_AI_STATE_WAIT_FOR_TARGET_HPP

#include "ai_state_impl.hpp"
#include <string>

class AiStateWaitForTarget : public AiStateImpl {
public:
    void update(float elapsed, Enemy* base) override;
    void setNextState(std::string const& nextState);

    void setDetectRange(float range);

private:
    std::string m_nextState = { "" };
    float m_detectRange { 100.0f };
};

#endif // ALAKAJAM14_AI_STATE_WAIT_FOR_TARGET_HPP
