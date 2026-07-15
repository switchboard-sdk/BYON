#pragma once

#include <switchboard_core/Extension.hpp>

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

