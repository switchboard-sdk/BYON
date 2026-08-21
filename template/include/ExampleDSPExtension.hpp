#pragma once

#include <optional>
#include <string>

#include <switchboard_core/Extension.hpp>

// C entry point resolved via dlsym when the extension is loaded dynamically.
// Its only job is to register the extension with the ExtensionManager — the
// same thing load() does for a statically-linked extension. Takes no arguments.
#if !defined(SWITCHBOARD_WEB)
extern "C" {
    void sb_extension_load();
}
#endif

namespace switchboard::extensions::exampledsp {

class ExampleDSPExtension final : public Extension {
public:
    static void load();

    // Overridden methods

    Result<void> initialize(const SBAnyMap& config) override;

    std::string getName() override;
    std::string getDescription() override;
    std::shared_ptr<NodeFactory> getNodeFactory() override;

    // Lets nodes fall back to a value from config.json when their own config omits the key.
    static std::optional<std::string> getConfigValue(const std::string& key);

private:
    static SBAnyMap extensionConfig;
};

}
