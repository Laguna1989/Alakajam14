#include "key.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "stairs.hpp"
#include "state_game.hpp"
Key::Key(jt::Vector2f const& pos, StateGame& state)
    : m_pos(pos)
    , m_state(state)
{
    std::cout << "key: " << m_pos.x << ", " << m_pos.y << std::endl;
}
void Key::doCreate()
{
    m_sprite = std::make_shared<jt::Sprite>("assets/key.png", getGame()->gfx().textureManager());
    m_sprite->setPosition(m_pos);
}
void Key::doUpdate(const float elapsed)
{
    m_sprite->update(elapsed);
    auto player = m_state.getPlayer();
    auto door = m_state.getStairs();

    float d = jt::MathHelper::length(m_pos
        + jt::Vector2f { m_sprite->getLocalBounds().height / 2.0f,
            m_sprite->getLocalBounds().width / 2.0f }
        - player->getPosition() + GP::PlayerSize() / 2.0f);

    if (d < 20.0f) {
        door->unlock();
        // TODO: Play sound
        m_sprite->setColor(jt::colors::Transparent);
    }
}
void Key::doDraw() const { m_sprite->draw(getGame()->gfx().target()); }
