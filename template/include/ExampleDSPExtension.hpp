#pragma once

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

    std::string getName() override;
    std::string getDescription() override;
    std::shared_ptr<NodeFactory> getNodeFactory() override;
};

}
