#ifndef ALAKAJAM14_PLAYER_GRAPHICS_COMPONENT_HPP
#define ALAKAJAM14_PLAYER_GRAPHICS_COMPONENT_HPP

#include "animation.hpp"
#include "game_interface.hpp"
#include "graphics_component_interface.hpp"
#include <memory>

class PlayerGraphicsComponent : public GraphicsComponentInterface {
public:
    explicit PlayerGraphicsComponent(std::shared_ptr<jt::GameInterface> gameInterface);

    void updateGraphics(float elapsed) override;

private:
    void createAnimation(jt::TextureManagerInterface& textureManager);
    std::shared_ptr<jt::Animation> m_animation;
    std::shared_ptr<jt::Animation> m_attackUnderlay;
};

#endif // ALAKAJAM14_PLAYER_GRAPHICS_COMPONENT_HPP
