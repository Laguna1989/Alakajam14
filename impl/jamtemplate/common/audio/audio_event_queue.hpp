#ifndef ALAKAJAM14_AUDIO_EVENT_QUEUE_HPP
#define ALAKAJAM14_AUDIO_EVENT_QUEUE_HPP

#include "audio_play_event.hpp"
#include <vector>

class AudioEventQueue {
public:

private:
    std::vector<AudioPlayEvent> m_buffer;
};

#endif // ALAKAJAM14_AUDIO_EVENT_QUEUE_HPP
