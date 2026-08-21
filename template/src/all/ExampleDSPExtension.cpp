#include "ExampleDSPExtension.hpp"

#include <switchboard_core/ExtensionManager.hpp>
#include "ExampleDSPNodeFactory.hpp"

#if !defined(SWITCHBOARD_WEB)
extern "C" void sb_extension_load() {
    switchboard::extensions::exampledsp::ExampleDSPExtension::load();
}
#endif

namespace switchboard::extensions::exampledsp {

SBAnyMap ExampleDSPExtension::extensionConfig;

void ExampleDSPExtension::load() {
    ExtensionManager::getInstance().registerExtension(std::make_shared<ExampleDSPExtension>());
}

Result<void> ExampleDSPExtension::initialize(const SBAnyMap& config) {
    // Only store the config here. Resolving an asset URI or opening a file this early fails:
    // initialize runs while the host is still starting up. See the README.
    extensionConfig = config;
    return makeSuccess();
}

std::optional<std::string> ExampleDSPExtension::getConfigValue(const std::string& key) {
    if (extensionConfig.hasKey(key)) {
        return extensionConfig.get<std::string>(key);
    }
    return std::nullopt;
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
