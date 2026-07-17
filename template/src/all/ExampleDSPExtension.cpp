#include "ExampleDSPExtension.hpp"

#include <switchboard_core/Logger.hpp>
#include <switchboard_core/ExtensionManager.hpp>
#include "ExampleDSPNodeFactory.hpp"

namespace switchboard::extensions::exampledsp {

void ExampleDSPExtension::load() {
    ExtensionManager::getInstance().registerExtension(std::make_shared<ExampleDSPExtension>());
}

std::string ExampleDSPExtension::getName() {
    return "ExampleDSP";
}

std::string ExampleDSPExtension::getDescription() {
    return "Example DSP extension demonstrating source, processor, and sink nodes.";
}

std::shared_ptr<NodeFactory> ExampleDSPExtension::getNodeFactory() {
    return std::make_shared<ExampleDSPNodeFactory>();
}

}
