#ifndef ALAKAJAM14_GUILE_HPP
#define ALAKAJAM14_GUILE_HPP

#include "animation.hpp"
#include "box2dwrapper/box2d_object.hpp"
#include "dialog/dialog_info.hpp"
#include "player/player.hpp"
#include "text.hpp"
#include <memory>

class Dialog;

class Guile : public jt::Box2DObject {
public:
    Guile(std::shared_ptr<jt::Box2DWorldInterface> world, b2BodyDef const* def,
        std::weak_ptr<TargetInterface> player);

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    void setDialog(DialogInfo const& info);

    std::weak_ptr<TargetInterface> m_target;

    std::shared_ptr<jt::Animation> m_animation;

private:
    std::shared_ptr<Dialog> m_dialog { nullptr };
    void drawDialog() const;
};

#endif // ALAKAJAM14_GUILE_HPP
