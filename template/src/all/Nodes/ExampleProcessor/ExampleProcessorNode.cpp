#include "ExampleProcessorNode.hpp"

namespace switchboard::extensions::exampledsp {

ExampleProcessorNode::ExampleProcessorNode(const SBAnyMap& config) : gain {1.0f} {
    const auto gainParam = SBAnyMap::get<float>(config, "gain", 1.0f);
    gain.store(gainParam);

    registerProperty(
        "gain",
        { { PROPERTY_FIELD_DESCRIPTION, "The gain of the audio signal. The range is [0, 1]." },
          { PROPERTY_FIELD_TYPE, PROPERTY_TYPE_FLOAT },
          { PROPERTY_FIELD_READ_ONLY, false },
          { PROPERTY_FIELD_MIN_VALUE, 0.0f },
          { PROPERTY_FIELD_MAX_VALUE, 1.0f },
          { PROPERTY_FIELD_DEFAULT_VALUE, gain.load() },
          { PROPERTY_FIELD_GETTER, std::function([this]() -> Result<SBAny> {
                return makeSuccess<SBAny>(gain.load());
            }) },
          { PROPERTY_FIELD_SETTER, std::function([this](const SBAny& value) -> Result<void> {
                const auto newGain = SBAny::convert<float>(value);
                gain.store(newGain);
                return makeSuccess();
            }) } }
    );
}

bool ExampleProcessorNode::setBusFormat(AudioBusFormat& inputBusFormat, AudioBusFormat& outputBusFormat) {
    return AudioBusFormat::matchBusFormats(inputBusFormat, outputBusFormat);
}

bool ExampleProcessorNode::process(AudioBus& inBus, AudioBus& outBus) {
    AudioBuffer<float>& inBuffer = *inBus.buffer;
    AudioBuffer<float>& outBuffer = *outBus.buffer;
    const uint numFrames = inBuffer.getNumberOfFrames();
    const uint numChannels = inBuffer.getNumberOfChannels();
    for (uint frameIndex = 0; frameIndex < numFrames; frameIndex++) {
        for (uint channelIndex = 0; channelIndex < numChannels; channelIndex++) {
            const float newValue = inBuffer.getSample(channelIndex, frameIndex) * gain.load();
            outBuffer.setSample(channelIndex, frameIndex, newValue);
        }
    }
    return true;
};

}
