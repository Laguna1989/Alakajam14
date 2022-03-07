#ifndef ALAKAJAM14_DIALOG_HPP
#define ALAKAJAM14_DIALOG_HPP

#include "dialog_info.hpp"
#include "game_object.hpp"
#include <functional>

class Dialog : public jt::GameObject {
public:
    Dialog(DialogInfo const& info);

    void setGiveSpellCallback(std::function<void(std::string const&)> func);

private:
    mutable DialogInfo m_dialogInfo;
    mutable std::string m_currentLineId { "" };
    mutable std::size_t m_currentIndex { 0U };
    std::function<void(std::string const&)> m_giveSpellCallback;

    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    DialogLine getCurrentLine() const;
    void nextMessageInLine() const;
    void drawSingleLine(DialogLine& d) const;
    void drawOptions(DialogLine const& d) const;
    void resetCurrentLine() const;
};

#endif // ALAKAJAM14_DIALOG_HPP
