#ifndef ALAKAJAM14_DEFERRED_ACTION_INTERFACE_HPP
#define ALAKAJAM14_DEFERRED_ACTION_INTERFACE_HPP

#include <functional>

class DeferredActionInterface {
public:
    virtual ~DeferredActionInterface() = default;
    virtual void queueDeferredAction(float time, std::function<void(void)> const& action) = 0;
};

#endif // ALAKAJAM14_DEFERRED_ACTION_INTERFACE_HPP
