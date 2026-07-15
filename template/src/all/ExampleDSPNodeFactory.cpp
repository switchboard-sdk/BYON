#include "ExampleDSPNodeFactory.hpp"

#include "ExampleSourceNode.hpp"
#include "ExampleProcessorNode.hpp"
#include "ExampleSinkNode.hpp"

namespace switchboard::extensions::exampledsp {

ExampleDSPNodeFactory::ExampleDSPNodeFactory() {
    registerNode(ExampleSourceNode::getNodeTypeInfo(), [](const SBAnyMap& config) {
        return new ExampleSourceNode(config);
    });
    registerNode(ExampleProcessorNode::getNodeTypeInfo(), [](const SBAnyMap& config) {
        return new ExampleProcessorNode(config);
    });
    registerNode(ExampleSinkNode::getNodeTypeInfo(), [](const SBAnyMap& config) {
        return new ExampleSinkNode(config);
    });
}

std::string ExampleDSPNodeFactory::getNodeTypePrefix() {
    return "ExampleDSP";
}

}
