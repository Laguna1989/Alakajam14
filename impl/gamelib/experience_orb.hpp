#ifndef ALAKAJAM14_EXPERIENCE_ORB_HPP
#define ALAKAJAM14_EXPERIENCE_ORB_HPP

#include "animation.hpp"
#include "audio/sound.hpp"
#include "box2dwrapper/box2d_object.hpp"
#include "vector.hpp"

class ExperienceOrb : public jt::Box2DObject {
public:
    ExperienceOrb(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def, int value);
    int m_value { 1 };
    bool m_pickedUp { false };

    void pickUp();

private:
    std::shared_ptr<jt::Animation> m_animation;
    std::shared_ptr<jt::Sound> m_soundBling;

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    void doKill() override;
};

#endif // ALAKAJAM14_EXPERIENCE_ORB_HPP
