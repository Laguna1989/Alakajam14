#ifndef ALAKAJAM14_STAIRS_H
#define ALAKAJAM14_STAIRS_H

#include "audio/sound_interface.hpp"
#include "game_object.hpp"
#include "sprite.hpp"
#include "state_game.hpp"

class Stairs : public jt::GameObject {
public:
    Stairs(StateGame& state);
    void setPosition(jt::Vector2f const& pos);
    void unlock();

private:
    bool m_locked = true;
    bool m_touched = false;
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    std::shared_ptr<jt::Sprite> m_sprite;
    StateGame& m_state;

    std::shared_ptr<jt::SoundInterface> m_soundUnlock;
};

#endif // ALAKAJAM14_STAIRS_H
