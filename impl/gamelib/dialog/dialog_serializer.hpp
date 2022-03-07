#ifndef ALAKAJAM14_DIALOG_SERIALIZER_HPP
#define ALAKAJAM14_DIALOG_SERIALIZER_HPP

#include "dialog_info.hpp"
#include "nlohmann.hpp"

void to_json(nlohmann::json& j, const DialogOption& p);
void to_json(nlohmann::json& j, const DialogLine& p);
void to_json(nlohmann::json& j, const DialogInfo& p);

void from_json(const nlohmann::json& j, DialogOption& p);
void from_json(const nlohmann::json& j, DialogLine& p);
void from_json(const nlohmann::json& j, DialogInfo& p);

#endif // ALAKAJAM14_DIALOG_SERIALIZER_HPP
