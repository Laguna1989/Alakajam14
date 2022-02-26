#include "game_properties.hpp"
jt::Palette GP::getPalette()
{
    return jt::Palette { { jt::Color { 5, 64, 71 }, jt::Color { 32, 142, 68 },
        jt::Color { 133, 225, 66 }, jt::Color { 239, 255, 124 }, jt::Color { 251, 114, 48 },
        jt::Color { 184, 39, 45 }, jt::Color { 64, 23, 58 }, jt::Color { 157, 64, 43 } } };
}

int GP::PhysicVelocityIterations() { return 20; }

int GP::PhysicPositionIterations() { return 20; }

jt::Vector2f GP::PlayerSize() { return jt::Vector2f { 16.0f, 16.0f }; }
float GP::PlayerBaseMovementSpeed() { return 85.0f; }
float GP::PlayerBaseDashVelocity() { return 500.0f; }
float GP::PlayerBaseDashCooldown() { return 1.4f; }
float GP::PlayerDashTotalTime() { return 0.6f; }
float GP::PlayerDashActiveTime() { return 0.05f; }
