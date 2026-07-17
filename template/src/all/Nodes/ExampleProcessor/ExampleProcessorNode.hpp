#pragma once

#include <switchboard_core/NodeTypeInfo.hpp>
#include <switchboard_core/SingleBusAudioProcessorNode.hpp>

namespace switchboard::extensions::exampledsp {

class ExampleProcessorNode : public SingleBusAudioProcessorNode {
public:
    static NodeTypeInfo getNodeTypeInfo() {
        return NodeTypeInfo { SWITCHBOARD_NODE_NAMESPACE,
                              "ExampleProcessor",
                              "ExampleProcessor",
                              "Example processor node.",
                              { NODE_CATEGORY_AUDIO_PROCESSING } };
    }

    explicit ExampleProcessorNode(const SBAnyMap& config);

    ~ExampleProcessorNode() override = default;

#pragma mark Overridden methods

    bool setBusFormat(AudioBusFormat& inputBusFormat, AudioBusFormat& outputBusFormat) override;
    bool process(AudioBus& inBus, AudioBus& outBus) override;

private:
    std::atomic<float> gain;
};

}
