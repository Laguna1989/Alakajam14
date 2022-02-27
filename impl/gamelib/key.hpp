#ifndef ALAKAJAM14_KEY_HPP
#define ALAKAJAM14_KEY_HPP

#include "game_object.hpp"
#include "sprite.hpp"
#include "stairs.hpp"
class StateGame;
class Key : public jt::GameObject {

public:
    Key(jt::Vector2f const& pos, StateGame&);

private:
    bool m_locked = false;
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    std::shared_ptr<jt::Sprite> m_sprite;
    jt::Vector2f m_pos;
    StateGame& m_state;
};

#endif // ALAKAJAM14_KEY_HPP
