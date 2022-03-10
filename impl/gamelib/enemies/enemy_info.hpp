#ifndef ALAKAJAM14_ENEMY_INFO_HPP
#define ALAKAJAM14_ENEMY_INFO_HPP

#include "vector.hpp"
#include <string>
#include <vector>

struct AnimationInfo {
    std::string animationName { "" };
    std::string fileName { "" };
    jt::Vector2u imageSize { 0, 0 };
    std::vector<unsigned int> frameIndices {};
    float frameTimeInSeconds { 0.2f };
};

struct AiInfo {
    enum AiType { WAIT, FOLLOW, SHOOT, BOSS, BOSS_INJURED, BOSS_CRITICAL };
    std::string name { "" };
    std::string nextState { "" };
    AiType type;
    float range;
};

struct EnemyInfo {
    int experience { 0 };
    float hitpoints { 1.0f };
    float closeCombatDamage { 0.0f };
    float movementSpeed { 0.0f };
    float colliderRadius { 1.0f };
    std::vector<AnimationInfo> animations {};
    std::vector<AiInfo> ais {};
    bool isBoss { false };
};

#endif // ALAKAJAM14_ENEMY_INFO_HPP
