#ifndef ALAKAJAM14_ENEMY_SERIALIZATION_HPP
#define ALAKAJAM14_ENEMY_SERIALIZATION_HPP

#include "enemy_info.hpp"
#include "nlohmann.hpp"
#include "vector.hpp"

void to_json(nlohmann::json& j, const AnimationInfo& p);
void to_json(nlohmann::json& j, const AiInfo& p);
void to_json(nlohmann::json& j, const EnemyInfo& p);

void from_json(const nlohmann::json& j, AnimationInfo& p);
void from_json(const nlohmann::json& j, AiInfo& p);
void from_json(const nlohmann::json& j, EnemyInfo& p);

#endif // ALAKAJAM14_ENEMY_SERIALIZATION_HPP
