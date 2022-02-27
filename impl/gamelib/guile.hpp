#ifndef ALAKAJAM14_GUILE_HPP
#define ALAKAJAM14_GUILE_HPP

#include "animation.hpp"
#include "box2dwrapper/box2d_object.hpp"
#include "character.hpp"
#include <memory>

class Guile : public jt::Box2DObject {
public:
    Guile(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
        std::weak_ptr<PlayerCharacter> player);
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::weak_ptr<PlayerCharacter> m_player;

    std::shared_ptr<jt::Animation> m_animation;

    bool m_hasGivenSpell { false };
    std::string m_spellToGive { "" };
};

#endif // ALAKAJAM14_GUILE_HPP
