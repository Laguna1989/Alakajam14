#ifndef ALAKAJAM14_AI_STATE_MANAGER_HPP
#define ALAKAJAM14_AI_STATE_MANAGER_HPP

#include "ai_state_interface.hpp"
#include <map>
#include <memory>
#include <string>

class AiStateManager {
public:
    void registerState(std::string const& identifier, std::shared_ptr<AiStateInterface> state);
    void switchToState(std::string const& identifier);

    std::shared_ptr<AiStateInterface> getCurrentState();
    void update();

private:
    std::string m_currentState { "" };
    std::string m_nextState { "" };
    std::map<std::string, std::shared_ptr<AiStateInterface>> m_states;
};

#endif // ALAKAJAM14_AI_STATE_MANAGER_HPP
