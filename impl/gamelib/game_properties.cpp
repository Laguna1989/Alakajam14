#include "game_properties.hpp"
jt::Palette GP::getPalette()
{
    return jt::Palette { { jt::Color { 5, 64, 71 }, jt::Color { 32, 142, 68 },
        jt::Color { 133, 225, 66 }, jt::Color { 239, 255, 124 }, jt::Color { 251, 114, 48 },
        jt::Color { 184, 39, 45 }, jt::Color { 64, 23, 58 }, jt::Color { 157, 64, 43 } } };
}

float GP::GetZoom() { return 2.0f; }
jt::Vector2f GP::GetWindowSize() { return jt::Vector2f { 800, 600 }; }
jt::Vector2f GP::GetScreenSize() { return GetWindowSize() * (1.0f / GetZoom()); }

int GP::PhysicVelocityIterations() { return 20; }

int GP::PhysicPositionIterations() { return 20; }

jt::Vector2f GP::PlayerSize() { return jt::Vector2f { 16.0f, 16.0f }; }
float GP::PlayerBaseMovementSpeed() { return 75.0f; }
float GP::PlayerBaseDashVelocity() { return 500.0f; }
float GP::PlayerBaseDashCooldown() { return 1.4f; }
float GP::PlayerDashTotalTime() { return 0.29f; }
float GP::PlayerDashActiveTime() { return 0.069f; }

float GP::ExperienceOrbVelocity() { return 45.0f; }
float GP::ExperienceOrbIdleTime() { return 0.6f; }
float GP::ExperienceOrbPickupDistance() { return 8.0f; }
float GP::ExperienceOrbAttractDistance() { return 28.0f; }
float GP::PlayerAttackCooldown() { return 0.6f; }
float GP::SpellSnipeProjectileSpeed() { return 250.0f; }
