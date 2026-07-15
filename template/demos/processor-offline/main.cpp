#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <switchboard/Switchboard.hpp>

#include "ExampleDSPExtension.hpp"

using namespace switchboard;
using namespace switchboard::extensions::exampledsp;

static std::optional<std::string> readContentsOfTextFile(const std::string& filePath) {
    std::filesystem::path fileSystemPath(filePath);
    if (!std::filesystem::exists(filePath)) {
        return std::nullopt;
    }
    std::ifstream fileStream(filePath);
    std::string fileContent((std::istreambuf_iterator(fileStream)), std::istreambuf_iterator<char>());
    return fileContent;
}

int main(int argc, const char* argv[]) {
    // Load JSON
    std::string engineJSONFilePath = "ProcessorOfflineDemo.json";
    auto engineJSON = readContentsOfTextFile(engineJSONFilePath);
    if (!engineJSON.has_value()) {
        std::cerr << "Failed to read engine JSON file: " << engineJSONFilePath << std::endl;
        return 1;
    }

    // Init Switchboard SDK and extensions
    ExampleDSPExtension::load();
    SBAnyMap sdkConfig({
        { "appID", "demo" },
        { "appSecret", "demo" },
        { "tempDirPath", "/tmp/switchboard" },
        { "extensions", SBAnyMap({
            { "ExampleDSP", SBAnyMap() }
        })}
    });
    Switchboard::initialize(sdkConfig);

    // Create audio engine
    Result<Switchboard::ObjectID> result = Switchboard::createEngine(engineJSON.value());
    if (result.isError()) {
        std::cerr << "Failed to create engine: " << result.error().message << std::endl;
        return 1;
    }
    const std::string engineID = result.value();

    // Process audio engine
    auto processEngineResult = Switchboard::callAction(engineID, "process", {});
    if (processEngineResult.isError()) {
        std::cerr << "Failed to start engine: " << processEngineResult.error().message << std::endl;
        return 1;
    }

    // Stop and tear down audio engine
    Switchboard::callAction(engineID, "stop", {});
    Switchboard::destroyEngine(engineID);
    return 0;
}