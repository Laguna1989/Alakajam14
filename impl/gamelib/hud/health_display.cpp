#include "health_display.hpp"

HealthDisplay::HealthDisplay(std::shared_ptr<jt::Bar> bar)
    : m_bar { bar }
{
}

void HealthDisplay::notify(float value) { m_bar->setCurrentValue(value); }
