#ifndef GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD
#define GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD

#include "color.hpp"
#include "vector.hpp"
#include <palette.hpp>
#include <string>

class GP {
public:
    GP() = delete;

    static std::string GameName() { return "MyAwesomeGame"; }
    static std::string AuthorName() { return "TODO"; }
    static std::string JamName() { return "TODO"; }
    static std::string JamDate() { return "TODO"; }

    static jt::Vector2f GetWindowSize();
    static float GetZoom();
    static jt::Vector2f GetScreenSize();

    static jt::Color PaletteBackground() { return GP::getPalette().getColor(5); }
    static jt::Color PaletteFontFront() { return GP::getPalette().getColor(3); }
    static jt::Color PaletteFontShadow() { return GP::getPalette().getColor(6); }

    static jt::Palette getPalette();

    static int PhysicVelocityIterations();
    static int PhysicPositionIterations();

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
    static float EnemyCrystallSmallHitPoints();
    static float EnemyCrystallMediumHitPoints();
    static float EnemyCrystallLargeHitPoints();
    static int LootExperienceMediumAmount();
    static int LootExperienceSmallAmount();
    static int LootExperienceLargeAmount();

    static float EnemyShotSpeed();
    static float EnemyShotTimer();
    static float EnemyShotRange();
    static float EnemyShotDamage();
    static float LevelUpHealthIncrease();
    static float LevelUpMovementSpeedIncrease();
    static float LevelUpAttackDamageIncrease();
};

#endif
