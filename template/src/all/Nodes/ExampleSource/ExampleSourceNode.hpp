#pragma once

#include <switchboard_core/NodeTypeInfo.hpp>
#include <switchboard_core/SingleBusAudioSourceNode.hpp>

namespace switchboard::extensions::exampledsp {

class ExampleSourceNode : public SingleBusAudioSourceNode {
public:
    static NodeTypeInfo getNodeTypeInfo() {
        return NodeTypeInfo { SWITCHBOARD_NODE_NAMESPACE,
                              "ExampleSource",
                              "ExampleSource",
                              "Example source node.",
                              { NODE_CATEGORY_AUDIO_PROCESSING } };
    }

    explicit ExampleSourceNode(const SBAnyMap& config);

    ~ExampleSourceNode() override = default;

#pragma mark Overridden methods

    bool setBusFormat(AudioBusFormat& busFormat) override;
    bool produce(AudioBus& bus) override;

private:
    std::atomic<float> frequency;
    std::atomic<float> amplitude;
    float phase;
};

}
