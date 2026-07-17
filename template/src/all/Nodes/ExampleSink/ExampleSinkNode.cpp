#include "ExampleSinkNode.hpp"
#include <cmath>

namespace switchboard::extensions::exampledsp {

ExampleSinkNode::ExampleSinkNode(const SBAnyMap& config) : peakValue {0.0f}, eventIntervalFrames {48000}, frameCounter {0} {
    const auto eventIntervalFramesParams = SBAnyMap::get<unsigned int>(config, "eventIntervalFrames", 48000);
    eventIntervalFrames = eventIntervalFramesParams;

    registerProperty(
        "eventIntervalFrames",
        { { PROPERTY_FIELD_TYPE, PROPERTY_TYPE_INT },
      { PROPERTY_FIELD_READ_ONLY, false },
      { PROPERTY_FIELD_UNIT, PROPERTY_UNIT_MS },
      { PROPERTY_FIELD_DESCRIPTION, "Interval in frames between timer events." },
      { PROPERTY_FIELD_MIN_VALUE, 1 },
      { PROPERTY_FIELD_MAX_VALUE, 60000 },
      { PROPERTY_FIELD_GETTER, std::function([this]() -> SBAny {
            return this->eventIntervalFrames;
        }) },
      { PROPERTY_FIELD_SETTER, std::function([this](const SBAny& value) {
            this->eventIntervalFrames = SBAny::convert<unsigned int>(value);
        }) } }
    );

    registerEvent(
        "peak",
        { { EVENT_FIELD_NAME, "peak" },
            { EVENT_FIELD_DESCRIPTION, "Emitted when peak is detected in the input audio buffer." },
            { EVENT_FIELD_DATA,
            SBAnyMap(
                { { "peak",
                    SBAnyMap(
                        { { EVENT_DATA_FIELD_NAME, "peak" },
                          { EVENT_DATA_FIELD_TYPE, EVENT_DATA_TYPE_FLOAT },
                          { EVENT_DATA_FIELD_DESCRIPTION, "The peak value of input audio." } }
                    ) } }
            ) } }
    );
}

bool ExampleSinkNode::setBusFormat(AudioBusFormat& busFormat) {
    return true;
}

bool ExampleSinkNode::consume(AudioBus& bus) {
    AudioBuffer<float>& inBuffer = *bus.buffer;
    const uint numFrames = inBuffer.getNumberOfFrames();
    const uint numChannels = inBuffer.getNumberOfChannels();
    for (uint frameIndex = 0; frameIndex < numFrames; frameIndex++) {
        for (uint channelIndex = 0; channelIndex < numChannels; channelIndex++) {
            const float absSampleValue = fabsf(inBuffer.getSample(channelIndex, frameIndex));
            if (absSampleValue > peakValue) {
                peakValue = absSampleValue;
            }
        }
        frameCounter++;
        if (frameCounter == eventIntervalFrames) {
            const SBAnyMap eventData = { { "peak", peakValue } };
            emitEvent("peak", eventData);
            peakValue = 0.0f;
            frameCounter = 0;
        }
    }
    return true;
}

}

