#ifndef ALAKAJAM14_SOUND_COMPONENT_INTERFACE_HPP
#define ALAKAJAM14_SOUND_COMPONENT_INTERFACE_HPP

class SoundComponentInterface {
public:
    enum class SoundId { DASH, STOMP, HURT, DEATH };
    virtual ~SoundComponentInterface() = default;
    virtual void play(SoundId sound) = 0;

    virtual bool isPlaying(SoundId sound) = 0;
};

#endif // ALAKAJAM14_SOUND_COMPONENT_INTERFACE_HPP
