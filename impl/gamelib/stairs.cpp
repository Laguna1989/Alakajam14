#include "stairs.hpp"
#include "game_interface.hpp"
void Stairs::unlock() { m_locked = false; }
Stairs::Stairs(jt::Vector2f const& pos)
    : m_locked(true)
    , m_pos(pos)
{
}
void Stairs::doCreate()
{
    m_sprite = std::make_shared<jt::Sprite>("assets/door.png", getGame()->gfx().textureManager());
    m_sprite->setPosition(m_pos);
}
void Stairs::doUpdate(const float elapsed)
{
    if (m_locked) {
        m_sprite->update(elapsed);
    } else {
        m_sprite->setColor(jt::colors::Transparent);
    }
}
void Stairs::doDraw() const { m_sprite->draw(getGame()->gfx().target()); }
