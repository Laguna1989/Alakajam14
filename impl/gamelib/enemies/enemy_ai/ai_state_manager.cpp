#include "ai_state_manager.hpp"

void AiStateManager::registerState(
    std::string const& identifier, std::shared_ptr<AiStateInterface> state)
{

    m_states[identifier] = state;
}

void AiStateManager::switchToState(std::string const& identifier) { m_nextState = identifier; }

std::shared_ptr<AiStateInterface> AiStateManager::getCurrentState()
{
    return m_states[m_currentState];
}
void AiStateManager::update()
{
    if (m_nextState == "") {
        return;
    }

    m_currentState = m_nextState;
    m_nextState = "";
}
