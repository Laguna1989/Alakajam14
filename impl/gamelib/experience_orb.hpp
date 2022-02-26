#ifndef ALAKAJAM14_EXPERIENCE_ORB_HPP
#define ALAKAJAM14_EXPERIENCE_ORB_HPP

#include "animation.hpp"
#include "box2dwrapper/box2d_object.hpp"
#include "vector.hpp"

class ExperienceOrb : public jt::Box2DObject {
public:
    ExperienceOrb(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
        jt::Vector2f const& pos, int value);
    int m_value { 1 };

private:
    std::shared_ptr<jt::Animation> m_animation;

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    void doKill() override;
};

#endif // ALAKAJAM14_EXPERIENCE_ORB_HPP
