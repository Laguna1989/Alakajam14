#ifndef GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD
#define GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD

#include "color.hpp"
#include "vector.hpp"
#include <palette.hpp>
#include <string>

class GP {
public:
    GP() = delete;

    static std::string GameName() { return "Funky Trip"; }
    static std::string AuthorName()
    {
        return "adkiem, Aneira, BloodyOrange,\nLaguna, Synchronleuchter, Thunraz\n";
    }
    static std::string JamName() { return "Alakajam 14"; }
    static std::string JamDate() { return "February 2022"; }

    static jt::Vector2f GetWindowSize();
    static float GetZoom();
    static jt::Vector2f GetScreenSize();

    static jt::Color PaletteBackground() { return GP::getPalette().getColor(5); }
    static jt::Color PaletteFontFront() { return GP::getPalette().getColor(3); }
    static jt::Color PaletteFontShadow() { return GP::getPalette().getColor(6); }

    static jt::Palette getPalette();

    static int PhysicVelocityIterations();
    static int PhysicPositionIterations();
    static std::uint16_t PhysicsCollisionCategoryWalls();
    static std::uint16_t PhysicsCollisionCategoryPlayer();
    static std::uint16_t PhysicsCollisionCategoryEnemies();
    static std::uint16_t PhysicsCollisionCategoryPlayerShots();
    static std::uint16_t PhysicsCollisionCategoryEnemyShots();
    static std::uint16_t PhysicsCollisionCategoryExperienceOrbs();
    static std::uint16_t PhysicsCollisionCategoryHealingPotions();

    static jt::Vector2f PlayerSize();
    static float PlayerBaseMovementSpeed();
    static float PlayerBaseDashVelocity();
    static float PlayerDashTotalTime();
    static float PlayerDashActiveTime();

    static float PlayerBaseDashCooldown();
    static float ExperienceOrbVelocity();
    static float ExperienceOrbIdleTime();
    static float ExperienceOrbPickupDistance();
    static float ExperienceOrbAttractDistance();

    static float PlayerAttackCooldown();
    static float SpellSnipeProjectileSpeed();
    static int SpellSnipeExpCosts();
    static float SpellBroadProjectileSpeed();
    static int SpellBroadExpCosts();
    static int LootExperienceMediumAmount();
    static int LootExperienceSmallAmount();

    static int LootExperienceLargeAmount();
    static float EnemyShotSpeed();
    static float EnemyShotTimer();
    static float EnemyShotDamage();
    static float LevelUpHealthIncrease();
    static float LevelUpMovementSpeedIncrease();
    static float LevelUpAttackDamageIncrease();
    static float TileSizeInPixel();
    static float HealingPotionAmount();
    static float HealingPotionPickupDistance();
    static float LevelUpMagicDamageIncrease();
    static float SpellSnipeProjectileDamage();
    static float SpellBroadProjectileDamage();
};

#endif
