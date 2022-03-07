#ifndef ALAKAJAM14_DIALOG_INFO_HPP
#define ALAKAJAM14_DIALOG_INFO_HPP

#include <string>
#include <vector>

struct DialogOption {
    std::string text { "" };
    std::string next { "" };
    std::string spellToGive { "" };
};

struct DialogLine {
    std::string identifier { "0" };
    std::vector<std::string> lines {};
    std::vector<DialogOption> options {};
};

struct DialogInfo {
    std::vector<DialogLine> lines;
};

#endif // ALAKAJAM14_DIALOG_INFO_HPP
