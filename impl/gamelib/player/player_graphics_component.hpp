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
    void setPosition(jt::Vector2f const& playerPosition) override;

    void draw(std::shared_ptr<jt::RenderTarget> target) override;
    void flash(float time, jt::Color const& color) override;
    bool setAnimationIfNotSet(std::string const& newAnimationName) override;
    void setPlayerAnimationLooping(bool isLooping) override;
    void setUnderlayAnimation(std::string const& animationName) override;

    std::string getCurrentAnimation() const override;

private:
    void createAnimation(jt::TextureManagerInterface& textureManager);
    std::shared_ptr<jt::Animation> m_animation;
    std::shared_ptr<jt::Animation> m_attackUnderlay;
};

#endif // ALAKAJAM14_PLAYER_GRAPHICS_COMPONENT_HPP
