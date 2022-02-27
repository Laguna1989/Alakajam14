#ifndef ALAKAJAM14_HEALTH_DISPLAY_HPP
#define ALAKAJAM14_HEALTH_DISPLAY_HPP

#include "bar.hpp"
#include "observer_interface.hpp"
#include <memory>

class HealthDisplay : public ObserverInterface<float> {
public:
    HealthDisplay(std::shared_ptr<jt::Bar> bar);
    void notify(float value) override;

private:
    std::shared_ptr<jt::Bar> m_bar { nullptr };
};

class HealthMaxDisplay : public ObserverInterface<float> {
public:
    HealthMaxDisplay(std::shared_ptr<jt::Bar> bar);
    void notify(float value) override;

private:
    std::shared_ptr<jt::Bar> m_bar { nullptr };
};

#endif // ALAKAJAM14_HEALTH_DISPLAY_HPP
