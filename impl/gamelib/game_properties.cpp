#include "game_properties.hpp"
jt::Palette GP::getPalette()
{
    return jt::Palette { { jt::Color { 20, 20, 20 }, jt::Color { 32, 142, 68 },
        jt::Color { 133, 225, 66 }, jt::Color { 239, 255, 124 }, jt::Color { 255, 97, 178 },
        jt::Color { 184, 39, 45 }, jt::Color { 64, 23, 58 }, jt::Color { 219, 65, 97 } } };
}

float GP::GetZoom() { return 2.0f; }
jt::Vector2f GP::GetWindowSize() { return jt::Vector2f { 800, 600 }; }
jt::Vector2f GP::GetScreenSize() { return GetWindowSize() * (1.0f / GetZoom()); }

int GP::PhysicVelocityIterations() { return 20; }
int GP::PhysicPositionIterations() { return 20; }

std::uint16_t GP::PhysicsCollisionCategoryWalls() { return 0x0001; }
std::uint16_t GP::PhysicsCollisionCategoryPlayer() { return 0x0002; }
std::uint16_t GP::PhysicsCollisionCategoryEnemies() { return 0x0004; }
std::uint16_t GP::PhysicsCollisionCategoryPlayerShots() { return 0x0008; }
std::uint16_t GP::PhysicsCollisionCategoryEnemyShots() { return 0x0010; }
std::uint16_t GP::PhysicsCollisionCategoryExperienceOrbs() { return 0x0020; }
std::uint16_t GP::PhysicsCollisionCategoryHealingPotions() { return 0x0040; }

jt::Vector2f GP::PlayerSize() { return jt::Vector2f { 16.0f, 16.0f }; }
float GP::PlayerBaseMovementSpeed() { return 55.0f; }
float GP::PlayerBaseDashVelocity() { return 500.0f; }
float GP::PlayerBaseDashCooldown() { return 1.4f; }
float GP::PlayerDashTotalTime() { return 0.29f; }
float GP::PlayerDashActiveTime() { return 0.069f; }
float GP::PlayerAttackCooldown() { return 0.6f; }

float GP::ExperienceOrbVelocity() { return 55.0f; }
float GP::ExperienceOrbIdleTime() { return 0.4f; }
float GP::ExperienceOrbPickupDistance() { return 22.0f; }
float GP::ExperienceOrbAttractDistance() { return 36.0f; }

float GP::SpellSnipeProjectileSpeed() { return 250.0f; }
float GP::SpellSnipeProjectileDamage() { return 200.0f; }
int GP::SpellSnipeExpCosts() { return 10; }
float GP::SpellBroadProjectileSpeed() { return 160.0f; }
float GP::SpellBroadProjectileDamage() { return 80.0; }
int GP::SpellBroadExpCosts() { return 20; }

int GP::LootExperienceSmallAmount() { return 15; }
int GP::LootExperienceMediumAmount() { return 35; }
int GP::LootExperienceLargeAmount() { return 75; }

float GP::EnemyShotSpeed() { return 125.0f; }
float GP::EnemyShotTimer() { return 1.2f; }
float GP::EnemyShotDamage() { return 10.0f; }

float GP::LevelUpHealthIncrease() { return 7.0f; }
float GP::LevelUpMovementSpeedIncrease() { return 0.035f; }
float GP::LevelUpAttackDamageIncrease() { return 4.0f; }
float GP::LevelUpMagicDamageIncrease() { return 4.0f; }

float GP::TileSizeInPixel() { return 16.0f; }
float GP::HealingPotionAmount() { return 15.0f; }
float GP::HealingPotionPickupDistance() { return GP::ExperienceOrbPickupDistance(); }
