#include "key.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"
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
    float d = jt::MathHelper::length(m_pos - player->getPosition());
    if (d < 8.0f) {
        door->unlock();
        // TODO: Play sound
        m_sprite->setColor(jt::colors::Transparent);
    }
}
void Key::doDraw() const { m_sprite->draw(getGame()->gfx().target()); }
