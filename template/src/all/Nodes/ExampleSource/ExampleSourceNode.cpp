#include "ExampleSourceNode.hpp"

#include <cmath>
#include <switchboard_core/Logger.hpp>

namespace switchboard::extensions::exampledsp {

ExampleSourceNode::ExampleSourceNode(const SBAnyMap& config) : frequency {440.0f}, amplitude {0.8f}, phase {0.0f} {
    const auto frequencyParam = SBAnyMap::get<float>(config, "frequency", 440.0f);
    frequency.store(frequencyParam);

    const auto amplitudeParam = SBAnyMap::get<float>(config, "amplitude", 1.0f);
    amplitude.store(amplitudeParam);

    registerProperty(
    "frequency",
    { { PROPERTY_FIELD_DESCRIPTION, "The frequency of the sine wave in Hz." },
      { PROPERTY_FIELD_TYPE, PROPERTY_TYPE_FLOAT },
      { PROPERTY_FIELD_READ_ONLY, false },
      { PROPERTY_FIELD_MIN_VALUE, 0.0f },
      { PROPERTY_FIELD_MAX_VALUE, 22000.0f },
      { PROPERTY_FIELD_DEFAULT_VALUE, frequency.load() },
      { PROPERTY_FIELD_GETTER, std::function([this]() -> Result<SBAny> {
            return makeSuccess<SBAny>(frequency.load());
        }) },
      { PROPERTY_FIELD_SETTER, std::function([this](const SBAny& value) -> Result<void> {
            const auto newFrequency = SBAny::convert<float>(value);
            frequency.store(newFrequency);
            return makeSuccess();
        }) } }
);

    registerProperty(
        "amplitude",
        { { PROPERTY_FIELD_DESCRIPTION, "The amplitude of the sine wave in the range of [0, 1]." },
          { PROPERTY_FIELD_TYPE, PROPERTY_TYPE_FLOAT },
          { PROPERTY_FIELD_READ_ONLY, false },
          { PROPERTY_FIELD_MIN_VALUE, 0.0f },
          { PROPERTY_FIELD_MAX_VALUE, 1.0f },
          { PROPERTY_FIELD_DEFAULT_VALUE, amplitude.load() },
          { PROPERTY_FIELD_GETTER, std::function([this]() -> Result<SBAny> {
                return makeSuccess<SBAny>(amplitude.load());
            }) },
          { PROPERTY_FIELD_SETTER, std::function([this](const SBAny& value) -> Result<void> {
                const auto newAmplitude = SBAny::convert<float>(value);
                amplitude.store(newAmplitude);
                return makeSuccess();
            }) } }
    );
}

bool ExampleSourceNode::setBusFormat(AudioBusFormat& busFormat) {
    return true;
}

bool ExampleSourceNode::produce(AudioBus& bus) {
    const uint numberOfChannels = bus.getBuffer()->getNumberOfChannels();
    const uint numberOfFrames = bus.getBuffer()->getNumberOfFrames();
    const uint sampleRate = bus.getBuffer()->getSampleRate();
    const float angularFrequency = 2.0f * M_PI * frequency / sampleRate;
    for (uint i = 0; i < numberOfFrames; i++) {
        const float sampleValue = amplitude * sinf(phase);
        phase += angularFrequency;
        if (phase >= 2.0f * M_PI) {
            phase -= 2.0f * M_PI;
        }
        for (uint channelIndex = 0; channelIndex < numberOfChannels; ++channelIndex) {
            float* floatBuffer = bus.getBuffer()->getWritePointer(channelIndex);
            floatBuffer[i] = sampleValue;
        }
    }
    return true;
}

}

