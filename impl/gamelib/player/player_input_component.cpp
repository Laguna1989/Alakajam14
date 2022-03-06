#include "player_input_component.hpp"
#include "game_properties.hpp"
#include "player.hpp"

PlayerInputComponent::PlayerInputComponent(
    std::shared_ptr<jt::KeyboardInputInterface> keyboardInterface)
    : m_keyboard { keyboardInterface }
{
}

void PlayerInputComponent::updateMovement(Player& player)
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

    if (m_keyboard->justPressed(jt::KeyCode::LShift)
        || m_keyboard->justPressed(jt::KeyCode::RShift)) {
        player.dash();
    }
}

void PlayerInputComponent::updateAttack(Player& player)
{
    if (m_keyboard->justPressed(jt::KeyCode::Space)
        || m_keyboard->justPressed(jt::KeyCode::Numpad0)) {
        player.attack();
    }
}
void PlayerInputComponent::updateSpells(Player& player)
{
    std::map<std::size_t, std::vector<jt::KeyCode>> lookup;

    lookup[0] = { jt::KeyCode::Q, jt::KeyCode::Num1, jt::KeyCode::Numpad1 };
    lookup[1] = { jt::KeyCode::E, jt::KeyCode::Num2, jt::KeyCode::Numpad2 };
    lookup[2] = { jt::KeyCode::Tab, jt::KeyCode::Num3, jt::KeyCode::Numpad3 };

    for (auto const& kvp : lookup) {
        for (auto const& key : kvp.second) {
            if (m_keyboard->justPressed(key)) {
                player.castSpell(kvp.first);
            }
        }
    }
}
