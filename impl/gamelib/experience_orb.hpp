#ifndef ALAKAJAM14_EXPERIENCE_ORB_HPP
#define ALAKAJAM14_EXPERIENCE_ORB_HPP

#include "box2dwrapper/box2d_object.hpp"
#include "target_interface.hpp"
#include "vector.hpp"

namespace jt {
class Animation;
class Sound;
} // namespace jt

class ExperienceOrb : public jt::Box2DObject {
public:
    ExperienceOrb(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, int value,
        std::weak_ptr<TargetInterface> target);
    int m_value { 1 };
    bool m_pickedUp { false };

    void pickUp();

private:
    std::shared_ptr<jt::Animation> m_animation;
    std::shared_ptr<jt::Sound> m_soundBling;
    std::shared_ptr<TargetInterface> m_target;

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    void doKill() override;
};

#endif // ALAKAJAM14_EXPERIENCE_ORB_HPP
