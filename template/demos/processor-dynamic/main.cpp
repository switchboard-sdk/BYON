// Processor demo that loads the extension DYNAMICALLY.
//
// The other processor demos link the extension at compile time and call
// ExampleDSPExtension::load() directly. This one instead loads the built
// shared library at runtime — exactly as the SDK's ExtensionLoader does:
// dlopen the .so, resolve the exported `sb_extension_load` symbol, and call it
// to register the extension. It then runs the same ExampleProcessor graph,
// offline or realtime.
//
// Usage:  ProcessorDynamicDemo [offline|realtime] [path/to/libSwitchboardExampleDSP.so]
//
// Defaults: mode "offline", library "./libSwitchboardExampleDSP.so".

#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <switchboard/Switchboard.hpp>

using namespace switchboard;

static std::optional<std::string> readContentsOfTextFile(const std::string& filePath) {
    if (!std::filesystem::exists(filePath)) {
        return std::nullopt;
    }
    std::ifstream fileStream(filePath);
    std::string fileContent((std::istreambuf_iterator(fileStream)), std::istreambuf_iterator<char>());
    return fileContent;
}

int main(int argc, const char* argv[]) {
    const std::string mode = argc > 1 ? argv[1] : "offline";
    const std::string extensionPath = argc > 2 ? argv[2] : "./libSwitchboardExampleDSP.so";
    if (mode != "offline" && mode != "realtime") {
        std::cerr << "Unknown mode '" << mode << "'. Use 'offline' or 'realtime'." << std::endl;
        return 1;
    }

    // 1. Load the extension shared library and resolve the C entry point.
    void* handle = dlopen(extensionPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!handle) {
        std::cerr << "dlopen failed: " << dlerror() << std::endl;
        return 1;
    }
    auto loadExtension = reinterpret_cast<void (*)()>(dlsym(handle, "sb_extension_load"));
    if (!loadExtension) {
        std::cerr << "Could not find 'sb_extension_load' symbol: " << dlerror() << std::endl;
        return 1;
    }
    loadExtension();  // registers the extension with the SDK
    std::cout << "Dynamically loaded extension from " << extensionPath << std::endl;

    // 2. Load the engine JSON for the requested mode.
    const std::string engineJSONFilePath =
        mode == "realtime" ? "ProcessorDynamicDemo-Realtime.json" : "ProcessorDynamicDemo-Offline.json";
    auto engineJSON = readContentsOfTextFile(engineJSONFilePath);
    if (!engineJSON.has_value()) {
        std::cerr << "Failed to read engine JSON file: " << engineJSONFilePath << std::endl;
        return 1;
    }

    // 3. Initialize the SDK. It only needs its own credentials (appID/appSecret);
    //    the ExampleDSP node needs no extension configuration.
    SBAnyMap sdkConfig({
        { "appID", "demo" },
        { "appSecret", "demo" },
        { "tempDirPath", "/tmp/switchboard" },
        { "extensions", SBAnyMap({
            { "ExampleDSP", SBAnyMap() }
        })} });
    if (Switchboard::initialize(sdkConfig).isError()) {
        std::cerr << "Failed to initialize the Switchboard SDK." << std::endl;
        return 1;
    }

    // 4. Create the audio engine from the graph JSON (uses the dynamically
    //    loaded ExampleDSP.ExampleProcessor node).
    Result<Switchboard::ObjectID> result = Switchboard::createEngine(engineJSON.value());
    if (result.isError()) {
        std::cerr << "Failed to create engine: " << result.error().message << std::endl;
        return 1;
    }
    const std::string engineID = result.value();

    if (mode == "realtime") {
        auto startResult = Switchboard::callAction(engineID, "start", {});
        if (startResult.isError()) {
            std::cerr << "Failed to start engine: " << startResult.error().message << std::endl;
            return 1;
        }
        std::cout << "Press any key to stop the engine..." << std::endl;
        std::cin.get();
        Switchboard::callAction(engineID, "stop", {});
    } else {
        auto processResult = Switchboard::callAction(engineID, "process", {});
        if (processResult.isError()) {
            std::cerr << "Failed to process engine: " << processResult.error().message << std::endl;
            return 1;
        }
        Switchboard::callAction(engineID, "stop", {});
        std::cout << "Processed input.wav -> output.wav" << std::endl;
    }

    Switchboard::destroyEngine(engineID);
    return 0;
}
