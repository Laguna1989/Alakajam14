#include "player_movement_component.hpp"
#include "game_properties.hpp"
#include "player.hpp"

PlayerMovementComponent::PlayerMovementComponent(
    std::shared_ptr<jt::KeyboardInputInterface> keyboardInterface)
    : m_keyboard { keyboardInterface }
{
}

void PlayerMovementComponent::update(Player& player)
{
    player.setVelocity(jt::Vector2f { 0.0f, 0.0f });

    auto const speed
        = GP::PlayerBaseMovementSpeed() * player.getCharSheet()->getMovementSpeedFactor();

    if (m_keyboard->pressed(jt::KeyCode::D) || m_keyboard->pressed(jt::KeyCode::Right)) {
        player.addVelocity(jt::Vector2f { speed, 0.0f });
    }
    if (m_keyboard->pressed(jt::KeyCode::A) || m_keyboard->pressed(jt::KeyCode::Left)) {
        player.addVelocity(jt::Vector2f { -speed, 0.0f });
    }

    if (m_keyboard->pressed(jt::KeyCode::W) || m_keyboard->pressed(jt::KeyCode::Up)) {
        player.addVelocity(jt::Vector2f { 0.0f, -speed });
    }
    if (m_keyboard->pressed(jt::KeyCode::S) || m_keyboard->pressed(jt::KeyCode::Down)) {
        player.addVelocity(jt::Vector2f { 0.0f, speed });
    }
}
