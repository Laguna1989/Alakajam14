﻿#ifndef EXPERIENCE_DISPLAY_GUARD
#define EXPERIENCE_DISPLAY_GUARD

#include "observer_interface.hpp"
#include "text.hpp"

class ExperienceDisplay : public ObserverInterface<int> {
public:
    explicit ExperienceDisplay(jt::Text::Sptr text, std::string const& prefix);
    void notify(int value) override;

private:
    jt::Text::Sptr const m_scoreText;
    std::string m_textPrefix;
};

#endif // !EXPERIENCE_DISPLAY_GUARD
