#ifndef ALAKAJAM14_KEY_HPP
#define ALAKAJAM14_KEY_HPP

#include "game_object.hpp"
#include "vector.hpp"

namespace jt {
class Sound;
class Sprite;
} // namespace jt

class StateGame;
class Key : public jt::GameObject {

public:
    Key(StateGame& state);
    void setPosition(jt::Vector2f const& pos);

private:
    bool m_locked { true };

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    std::shared_ptr<jt::Sprite> m_sprite;

    std::shared_ptr<jt::Sound> m_soundPickup;
    std::shared_ptr<jt::Sound> m_soundDoorUnlock;

    jt::Vector2f m_pos;
    StateGame& m_state;
};

#endif // ALAKAJAM14_KEY_HPP
