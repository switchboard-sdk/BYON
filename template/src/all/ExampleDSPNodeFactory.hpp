#pragma once

#include <switchboard_core/NodeFactory.hpp>

namespace switchboard::extensions::exampledsp {

class ExampleDSPNodeFactory : public NodeFactory {
public:
    ExampleDSPNodeFactory();

    // MARK: Overridden methods

    std::string getNodeTypePrefix() override;
};

}