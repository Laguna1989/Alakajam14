#ifndef ALAKAJAM14_GRAPHICS_COMPONENT_INTERFACE_HPP
#define ALAKAJAM14_GRAPHICS_COMPONENT_INTERFACE_HPP

class GraphicsComponentInterface {
public:
    virtual ~GraphicsComponentInterface() = default;
    virtual void updateGraphics(float elapsed) = 0;
};

#endif // ALAKAJAM14_GRAPHICS_COMPONENT_INTERFACE_HPP
