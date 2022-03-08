#include "dialog_serializer.hpp"

void to_json(nlohmann::json& j, DialogOption const& d)
{
    j = nlohmann::json { { "text", d.text }, { "next", d.next }, { "spell", d.spellToGive } };
}
void to_json(nlohmann::json& j, DialogLine const& d)
{
    j = nlohmann::json { { "options", d.options }, { "identifier", d.identifier },
        { "lines", d.lines } };
}
void to_json(nlohmann::json& j, DialogInfo const& d)
{
    j = nlohmann::json { { "dialog", d.lines } };
}

void from_json(nlohmann::json const& j, DialogOption& d)
{
    j.at("text").get_to(d.text);
    j.at("next").get_to(d.next);
    
    if (j.count("spell") != 0) {
        j.at("spell").get_to(d.spellToGive);
    }
}
void from_json(nlohmann::json const& j, DialogLine& d)
{
    j.at("identifier").get_to(d.identifier);
    j.at("lines").get_to(d.lines);
    if (j.count("options") != 0) {
        j.at("options").get_to(d.options);
    }
}
void from_json(nlohmann::json const& j, DialogInfo& d) { j.at("dialog").get_to(d.lines); }
