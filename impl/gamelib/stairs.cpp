#include "stairs.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
void Stairs::unlock() { m_locked = false; }
Stairs::Stairs(jt::Vector2f const& pos, StateGame& state)
    : m_locked(true)
    , m_touched(false)
    , m_pos(pos)
    , m_state(state)
{
}
void Stairs::doCreate()
{
    m_sprite = std::make_shared<jt::Sprite>("assets/door.png", getGame()->gfx().textureManager());
    m_sprite->setPosition(m_pos);
}
void Stairs::doUpdate(const float elapsed)
{
    m_sprite->update(elapsed);
    auto player = m_state.getPlayer();
    float d = jt::MathHelper::length(m_pos
        + jt::Vector2f { m_sprite->getLocalBounds().height / 2.0f,
            m_sprite->getLocalBounds().width / 2.0f }
        - player->getPosition() + GP::PlayerSize() / 2.0f);
    if (!m_locked && !m_touched && d < 40.0f) {
        m_touched = true;
        m_sprite->setColor(jt::colors::Transparent);
        // TODO: play sound
    }
    if (!m_locked && m_touched && d < 20.0f) {
        // TODO: teleport fanciness
        auto dest = m_state.getStairsDest();
        jt::CamInterface& cam = m_state.getGame()->gfx().camera();
        cam.move(dest - cam.getCamOffset() - jt::Vector2f { 200, 150 });
        player->setPosition(dest);
    }
}
void Stairs::doDraw() const { m_sprite->draw(getGame()->gfx().target()); }