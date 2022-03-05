#include "key.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "stairs.hpp"
#include "state_game.hpp"

Key::Key(StateGame& state)
    : m_state(state)
{
}

void Key::doCreate()
{
    m_sprite = std::make_shared<jt::Sprite>("assets/key.png", getGame()->gfx().textureManager());

    m_soundPickup = std::make_shared<jt::Sound>("assets/sound/pickup_key.ogg");
    m_soundDoorUnlock = std::make_shared<jt::Sound>("assets/sound/door.ogg");
}

void Key::doUpdate(const float elapsed)
{
    m_sprite->setPosition(m_pos);
    m_sprite->update(elapsed);
    auto player = m_state.getPlayer();
    auto door = m_state.getStairs();

    auto const keyPosition = m_pos
        + jt::Vector2f { m_sprite->getLocalBounds().height / 2.0f,
              m_sprite->getLocalBounds().width / 2.0f };
    auto const playerPosition = player->getPosition();

    float d = jt::MathHelper::lengthSquared(keyPosition - playerPosition);

    if (d < 20.0f * 20.0f && m_locked) {
        door->unlock();
        m_locked = false;

        m_soundPickup->play();
        m_soundDoorUnlock->play();

        m_sprite->setColor(jt::colors::Transparent);
    }
}

void Key::doDraw() const { m_sprite->draw(getGame()->gfx().target()); }

void Key::setPosition(jt::Vector2f const& pos) { m_pos = pos; }
