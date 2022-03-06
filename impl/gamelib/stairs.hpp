#ifndef ALAKAJAM14_STAIRS_H
#define ALAKAJAM14_STAIRS_H

#include "game_object.hpp"
#include "sprite.hpp"
#include "state_game.hpp"
class Stairs : public jt::GameObject {
public:
    Stairs(StateGame& state);
    void setPosition(jt::Vector2f const& pos);
    void unlock();

private:
    bool m_locked = false;
    bool m_touched = false;
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    std::shared_ptr<jt::Sprite> m_sprite;
    jt::Vector2f m_pos;
    StateGame& m_state;
};

#endif // ALAKAJAM14_STAIRS_H
