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
float GP::PlayerBaseMovementSpeed() { return 55.0f; }
float GP::PlayerBaseDashVelocity() { return 500.0f; }
float GP::PlayerBaseDashCooldown() { return 1.4f; }
float GP::PlayerDashTotalTime() { return 0.29f; }
float GP::PlayerDashActiveTime() { return 0.069f; }
float GP::PlayerAttackCooldown() { return 0.6f; }

float GP::ExperienceOrbVelocity() { return 55.0f; }
float GP::ExperienceOrbIdleTime() { return 0.4f; }
float GP::ExperienceOrbPickupDistance() { return 8.0f; }
float GP::ExperienceOrbAttractDistance() { return 36.0f; }

float GP::SpellSnipeProjectileSpeed() { return 250.0f; }
int GP::SpellSnipeExpCosts() { return 10; }
float GP::SpellBroadProjectileSpeed() { return 150.0f; }
int GP::SpellBroadExpCosts() { return 10; }

float GP::EnemyCrystallSmallHitPoints() { return 120.0f; }
float GP::EnemyCrystallMediumHitPoints() { return 80.0f; }
float GP::EnemyCrystallLargeHitPoints() { return 1000.0f; }
int GP::LootExperienceSmallAmount() { return 15; }
int GP::LootExperienceMediumAmount() { return 35; }
int GP::LootExperienceLargeAmount() { return 75; }

float GP::EnemyShotSpeed() { return 150.0f; }
float GP::EnemyShotTimer() { return 1.1f; }
float GP::EnemyShotRange() { return 250.0f; }
float GP::EnemyShotDamage() { return 10.0f; }
float GP::BossHitPoints() { return 20 * 1000.f; }
