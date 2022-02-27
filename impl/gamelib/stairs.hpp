#ifndef ALAKAJAM14_STAIRS_H
#define ALAKAJAM14_STAIRS_H

#include "game_object.hpp"
#include "sprite.hpp"
class Stairs : public jt::GameObject {
public:
    Stairs(jt::Vector2f const& pos);
    void unlock();

private:
    bool m_locked = false;
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    std::shared_ptr<jt::Sprite> m_sprite;
    jt::Vector2f m_pos;
};

#endif // ALAKAJAM14_STAIRS_H
