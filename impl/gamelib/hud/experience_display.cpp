#include "experience_display.hpp"

ExperienceDisplay::ExperienceDisplay(jt::Text::Sptr text, std::string const& prefix)
    : m_scoreText { text }
    , m_textPrefix { prefix }
{
}

void ExperienceDisplay::notify(int value)
{
    if (value >= 0) {
        m_scoreText->setText(m_textPrefix + std::to_string(value));
    }
}
