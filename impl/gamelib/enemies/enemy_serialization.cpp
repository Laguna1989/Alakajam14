#include "enemy_serialization.hpp"

void to_json(nlohmann::json& j, AnimationInfo const& p)
{
    j = nlohmann::json { { "animationName", p.animationName }, { "fileName", p.fileName },
        { "frameTimeInSeconds", p.frameTimeInSeconds }, { "frameIndices", p.frameIndices },
        { "imageSizeX", p.imageSize.x }, { "imageSizeY", p.imageSize.y } };
}

void to_json(nlohmann::json& j, AiInfo const& p)
{
    j = nlohmann::json { { "type", p.type }, { "range", p.range }, { "nextState", p.nextState },
        { "name", p.name } };
}

void to_json(nlohmann::json& j, EnemyInfo const& p)
{
    j = nlohmann::json { { "hitpoints", p.hitpoints }, { "experience", p.experience },
        { "movementSpeed", p.movementSpeed }, { "colliderRadius", p.colliderRadius },
        { "closeCombatDamage", p.closeCombatDamage }, { "ais", p.ais },
        { "animations", p.animations }, { "isBoss", p.isBoss } };
}

void from_json(nlohmann::json const& j, AnimationInfo& p)
{
    j.at("animationName").get_to(p.animationName);
    j.at("fileName").get_to(p.fileName);
    j.at("frameTimeInSeconds").get_to(p.frameTimeInSeconds);
    j.at("frameIndices").get_to(p.frameIndices);
    j.at("imageSizeX").get_to(p.imageSize.x);
    j.at("imageSizeY").get_to(p.imageSize.y);
}
void from_json(nlohmann::json const& j, AiInfo& p)
{
    j.at("type").get_to(p.type);
    j.at("range").get_to(p.range);
    j.at("nextState").get_to(p.nextState);
    j.at("name").get_to(p.name);
}
void from_json(nlohmann::json const& j, EnemyInfo& p)
{
    j.at("hitpoints").get_to(p.hitpoints);
    j.at("experience").get_to(p.experience);
    j.at("movementSpeed").get_to(p.movementSpeed);
    j.at("colliderRadius").get_to(p.colliderRadius);
    j.at("closeCombatDamage").get_to(p.closeCombatDamage);
    j.at("ais").get_to(p.ais);
    j.at("animations").get_to(p.animations);
    j.at("isBoss").get_to(p.isBoss);
}
