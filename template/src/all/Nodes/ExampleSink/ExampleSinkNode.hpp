#pragma once

#include <switchboard_core/NodeTypeInfo.hpp>
#include <switchboard_core/SingleBusAudioSinkNode.hpp>

namespace switchboard::extensions::exampledsp {

class ExampleSinkNode : public SingleBusAudioSinkNode {
public:
    static NodeTypeInfo getNodeTypeInfo() {
        return NodeTypeInfo { SWITCHBOARD_NODE_NAMESPACE,
                              "ExampleSink",
                              "ExampleSink",
                              "Example sink node.",
                              { NODE_CATEGORY_AUDIO_PROCESSING } };
    }

    explicit ExampleSinkNode(const SBAnyMap& config);

    ~ExampleSinkNode() override = default;

#pragma mark Overridden methods

    bool setBusFormat(AudioBusFormat& busFormat) override;
    bool consume(AudioBus& bus) override;

private:
    float peakValue;
    uint eventIntervalFrames;
    uint frameCounter;
};

}
