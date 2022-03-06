#include "stairs.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
void Stairs::unlock() { m_locked = false; }
Stairs::Stairs(StateGame& state)
    : m_locked(true)
    , m_touched(false)
    , m_state(state)
{
}

void Stairs::doCreate()
{
    m_sprite = std::make_shared<jt::Sprite>("assets/door.png", getGame()->gfx().textureManager());
}

void Stairs::doUpdate(const float elapsed)
{
    m_sprite->update(elapsed);
    auto player = m_state.getPlayer();
    float d = jt::MathHelper::lengthSquared(m_sprite->getPosition()
        + jt::Vector2f { m_sprite->getLocalBounds().height / 2.0f,
            m_sprite->getLocalBounds().width / 2.0f }
        - player->getPosition());
    if (!m_locked && !m_touched && d < 40.0f * 40.0f) {
        m_touched = true;
        m_sprite->setColor(jt::colors::Transparent);
        // TODO: play sound
    }
    if (!m_locked && m_touched && d < 20.0f * 20.0f) {
        // TODO: teleport fanciness
        auto dest = m_state.getStairsDest();
        jt::CamInterface& cam = getGame()->gfx().camera();
        cam.move(dest - cam.getCamOffset() - GP::GetScreenSize() * 0.5f);
        player->setPosition(dest);
    }
}
void Stairs::doDraw() const { m_sprite->draw(getGame()->gfx().target()); }
void Stairs::setPosition(jt::Vector2f const& pos) { m_sprite->setPosition(pos); }
