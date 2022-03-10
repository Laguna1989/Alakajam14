#ifndef ALAKAJAM14_HEALING_POTION_HPP
#define ALAKAJAM14_HEALING_POTION_HPP

#include "box2dwrapper/box2d_object.hpp"
#include "player/player.hpp"
#include "target_interface.hpp"
#include "vector.hpp"

namespace jt {
class Animation;
class SoundInterface;
} // namespace jt

class HealingPotion : public jt::Box2DObject {
public:
    HealingPotion(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
        std::weak_ptr<Player> target);
    bool m_pickedUp { false };

    void pickUp();

private:
    std::shared_ptr<jt::Animation> m_animation;
    std::shared_ptr<jt::SoundInterface> m_soundBling;
    std::shared_ptr<Player> m_target;

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    void doKill() override;
    void initSound();
};

#endif // ALAKAJAM14_HEALING_POTION_HPP
