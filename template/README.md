# ExampleDSP – a Switchboard SDK Extension

**ExampleDSP** is a custom C++ extension for the Switchboard SDK, generated from [BYON](https://github.com/switchboard-sdk/BYON). It ships with example source, processor, and sink nodes, runnable demos, and a ready-to-build CMake setup — replace the example nodes with your own audio processing.

---

## 🌐 Platform Support

| macOS | Windows | Linux | iOS         | Android     | watchOS     | JVM         | Web         |
| :---- | :------ | :---- | :---------- | :---------- | :---------- | :---------- | :---------- |
| ✅     | ✅       | ✅     | Coming soon | Coming soon | Coming soon | Coming soon | Coming soon |

---

## 🚀 Getting Started

We recommend compiling and running the provided code before making any changes. This will help you understand the structure and workflow of the project.

Key components:
- **`tasks.py`**: Contains [Invoke](https://www.pyinvoke.org/) commands for common development tasks.
- **CMake**: The project uses standard CMake tooling for cross-platform builds.

### 🔧 Example Nodes

This repository includes three example node implementations to demonstrate the architecture:

| Name               | Path                                                      | Description                                                       |
| ------------------ | --------------------------------------------------------- | ----------------------------------------------------------------- |
| `ExampleSource`    | `src/all/Nodes/ExampleSource/ExampleSourceNode.cpp`       | Generates a sine wave with adjustable frequency and amplitude.    |
| `ExampleProcessor` | `src/all/Nodes/ExampleProcessor/ExampleProcessorNode.cpp` | Applies a gain adjustment to the input signal.                    |
| `ExampleSink`      | `src/all/Nodes/ExampleSink/ExampleSinkNode.cpp`           | Measures peak input levels and emits events at defined intervals. |

Each node ships **offline** and **real-time** demos in the `demos/` directory:

- **Offline** demos use `OfflineGraphRenderer` to process an audio file and write an output file.
- **Real-time** demos use `RealtimeGraphRenderer` to process the system microphone and play it through the speakers.

Running them is the quickest way to see the extension working and to learn how nodes integrate into the SDK.

---

## 🏗 Building & Running the Demos

Build the extension and its demos with the platform-specific task (see `tasks.py` for the full list):

```bash
inv build-linux      # or: inv build-macos / inv build-windows
```

This compiles everything and installs the runnable demos — each alongside its graph JSON (and an `input.wav` for the offline demos) — into `out/bin/`. Run any demo from there:

```bash
cd out/bin
./ProcessorOfflineDemo     # applies gain to input.wav, writes output.wav
./ProcessorRealtimeDemo    # applies gain to the mic, plays to the speakers (press a key to stop)
```

| Demo binary | Node | What it does |
| --- | --- | --- |
| `SourceOfflineDemo` / `SourceRealtimeDemo` | `ExampleSource` | Generates a sine wave |
| `ProcessorOfflineDemo` / `ProcessorRealtimeDemo` | `ExampleProcessor` | Applies a gain adjustment |
| `SinkOfflineDemo` / `SinkRealtimeDemo` | `ExampleSink` | Measures peak input levels and emits events |

Offline demos operate on audio files in the current directory (the source/processor demos write an `output.wav`; the sink demo prints peak levels); real-time demos use the system microphone and speakers.

---

## 🔌 Dynamic Loading

The demos above link the extension at **compile time**. The same extension can also be built as
a shared library (`.so` / `.dylib`) and loaded **dynamically** at runtime. All this takes is one
exported C entry point:

```cpp
// ExampleDSPExtension.hpp
#if !defined(SWITCHBOARD_WEB)
extern "C" {
    void sb_extension_load();
}
#endif

// ExampleDSPExtension.cpp
extern "C" void sb_extension_load() {
    switchboard::extensions::exampledsp::ExampleDSPExtension::load();
}
```

The SDK's `ExtensionLoader` resolves `sb_extension_load` via `dlsym`. Its **only** job is to
**register** the extension with the `ExtensionManager` — exactly what a statically-linked
extension's `load()` does. It takes **no arguments**; nothing else is required to make an
extension dynamically loadable.

> Dynamic loading is supported on **macOS and desktop Linux** (matching the SDK's
> `ExtensionLoader`). The `processor-dynamic` demo below is built on those platforms only.

### Testing dynamic loading with the SDK

`ProcessorDynamicDemo` is the dynamic-loading counterpart of the `ProcessorOfflineDemo` /
`ProcessorRealtimeDemo` above — same `ExampleProcessor` graph, but it's the one demo that loads
the extension at **runtime** instead of linking it. It `dlopen`s the built `.so`, calls
`sb_extension_load`, then runs the graph offline or realtime.

After building (per the section above), verify the symbol and run it:

```bash
# Confirm the entry point is exported from the built library
nm -D build/libSwitchboardExampleDSP.so | grep sb_extension_load

# Run it — it loads the .so dynamically rather than linking it
cd out/bin
./ProcessorDynamicDemo offline    # applies gain to input.wav, writes output.wav
./ProcessorDynamicDemo realtime   # applies gain to the mic, plays to the speakers (press a key to stop)
```

The demo defaults to `./libSwitchboardExampleDSP.so` (installed next to it); pass a different
path as the second argument if your library lives elsewhere.

---

## 🧩 Adding Your Own Nodes

**One extension holds as many nodes as you like.** `ExampleSource`, `ExampleProcessor` and
`ExampleSink` are not three extensions — they are three nodes registered by the single
`ExampleDSPNodeFactory`, and you add a fourth the same way they were added. You don't need a
second extension to ship more nodes.

Every node is addressed as `<prefix>.<type>` — `ExampleDSP.ExampleProcessor`. The prefix comes
from `ExampleDSPNodeFactory::getNodeTypePrefix()` and is shared by every node in the extension;
the type comes from the node itself.

### Adding a node

Say you want a `Reverb` node. Four steps, all mirroring an existing node:

**1. Write the class** in `src/all/Nodes/Reverb/ReverbNode.{hpp,cpp}`, deriving from whichever
base matches its shape:

| Base class | For a node that | Must override |
| --- | --- | --- |
| `SingleBusAudioSourceNode` | produces audio (`ExampleSource`) | `setBusFormat(busFormat)`, `produce(bus)` |
| `SingleBusAudioProcessorNode` | transforms audio, in to out (`ExampleProcessor`) | `setBusFormat(in, out)`, `process(inBus, outBus)` |
| `SingleBusAudioSinkNode` | consumes audio (`ExampleSink`) | `setBusFormat(busFormat)`, `consume(bus)` |

Both overrides are pure virtual on every one of those bases, so a node missing either fails to
compile as an abstract class rather than failing at runtime.

Your constructor takes `const SBAnyMap& config` — that is what the factory hands it — but the
bases default-construct, so don't try to forward it: `SingleBusAudioProcessorNode(config)` won't
compile. Read your values out of `config` in the body instead, as `ExampleProcessorNode` does
with `SBAnyMap::get<float>(config, "gain", 1.0f)`.

**2. Declare its type info** as a static method on the class — this is what the node browser
lists, and it must be `static` because the factory calls it before any instance exists:

```cpp
static NodeTypeInfo getNodeTypeInfo() {
    return NodeTypeInfo { "ExampleDSP",              // prefix — same for every node here
                          "Reverb",                   // type, so "ExampleDSP.Reverb"
                          "Reverb",                   // display name
                          "Adds reverb to the input.",
                          { NODE_CATEGORY_EFFECTS } };
}
```

Categories are the `NODE_CATEGORY_*` constants in `NodeTypeInfo.hpp` — `AUDIO_PROCESSING`,
`EFFECTS`, `VOICE`, `MUSIC`, `MIXING`, `GENERATION`, `ANALYSIS`, `UTILITY`, `AI`, `RTC`. A node
may list more than one.

**3. Add it to the build** — a `CMakeLists.txt` in the node's own directory:

```cmake
target_include_directories(${TARGET_NAME} PRIVATE "./")
target_sources(${TARGET_NAME} PRIVATE "ReverbNode.cpp")
```

and one line in `src/all/Nodes/CMakeLists.txt`:

```cmake
add_subdirectory(Reverb)
```

**4. Register it with the factory** — add the include and one `registerNode` call in
`ExampleDSPNodeFactory`'s constructor:

```cpp
#include "ReverbNode.hpp"
// ...
registerNode(ReverbNode::getNodeTypeInfo(), [](const SBAnyMap& config) {
    return new ReverbNode(config);
});
```

**Step 4 is the one that actually makes the node exist.** A node that compiles but is never
registered is invisible: it won't appear in a node browser and `addNode` on its type fails. If
a new node doesn't show up, this is almost always why.

### Removing the example nodes

Nothing forces you to keep them, and dropping the ones you don't need keeps your node list
honest. To remove `ExampleSink`, reverse the steps above: delete
`src/all/Nodes/ExampleSink/`, its `add_subdirectory(ExampleSink)` line, its `registerNode` call
and `#include` in the factory, and the `demos/sink-*` directories plus their
`add_subdirectory` lines in `demos/CMakeLists.txt`. Leave the extension itself
(`ExampleDSPExtension`, `ExampleDSPNodeFactory`) in place — that is your extension, whatever
you end up naming your nodes.

---

<a id="configuring-your-extension-at-load-time-configjson"></a>

## ⚙️ Configuring your extension at load time (config.json)

When your extension is loaded **dynamically**, a `config.json` sitting next to the `.so` is how
it receives deployment-specific values — a model path, an endpoint, a tuning constant. Whatever
loads your extension parses that JSON and hands it to `initialize`:

```cpp
Result<void> initialize(const SBAnyMap& config) override;
```

`config.json` in this project starts as `{}`. Three things to know about what you put in it:

- **The keys are your extension's own.** `{"modelPath": "@user/my-model.onnx"}` — values your
  extension understands. It is **not** an `addNode` payload: `{"type": "ExampleDSP.MyNode",
  "config": {...}}` describes a node, and nothing inside it will ever reach one. Nested objects
  do parse (they arrive as nested `SBAnyMap`s), but the top-level keys are yours to define.
- **It does nothing unless you override `initialize`.** The base `Extension::initialize` ignores
  its argument, so an un-overridden extension is handed the parsed config and drops it. This
  project ships an override that stores the config and exposes it as
  `ExampleDSPExtension::getConfigValue("someKey")`, so a node can fall back to it when its own
  config omits the key (the SDK's `WaveshaperExtension::getDefaultModelPath` is the same pattern
  with fixed keys).
- **Node configuration does not come from here.** A visual editor's node browser calls `addNode`
  with the node *type* only, so a node that needs a path must get it from a C++ default, from
  the extension default your `initialize` stored, or from a property set before the graph starts.

Statically-linked builds — including every demo in this project — never read `config.json`. There
the config comes from the `extensions` map in the SDK's own init config, which the demos pass as
`{ "extensions": { "ExampleDSP": { ... } } }`. The file matters only on the dynamic path.

**Make `initialize` idempotent.** Registering an extension and configuring it are separate
steps, so `initialize` can be called more than once for the same extension, and an early call
can arrive with an empty config before the one carrying your `config.json`. Assigning what you
were given (as this project does) is safe; accumulating into existing state, or treating the
first call as the only one, is not.

---

## 📦 Packaging for Dynamic Loading

A dynamically loaded extension is delivered as a directory, not a single file. The build's
install step lays it out for you — no extra command:

```bash
inv build-linux
# out/extensions/
#   libSwitchboardExampleDSP.so
#   config.json          (copied from ./config.json)
```

That directory is the artifact — hand it over as-is to have your extension loaded. Two things
about its contents:

- **`config.json` travels with the library, in the same directory.** It is optional: a library
  with no config file beside it loads with an empty config. See
  [Configuring your extension at load time](#configuring-your-extension-at-load-time-configjson).
- **Node types come from your node factory's prefix**, not from the library's filename or from
  any path on disk. Renaming the `.so` changes nothing about how your nodes are addressed.

---

## 🗂 Assets (Models, Binaries, Audio)

Packaging carries the `.so` and `config.json` and nothing else. Models, `.bin` files and audio
are **assets**, and the SDK addresses them through URI aliases rather than absolute paths:

| Alias | What it is |
| --- | --- |
| `@user` | User-supplied files, such as a model your extension loads |
| `@system` | Files the host platform ships and owns |
| `@output` | Where a running graph writes its output |

An asset URI is what belongs in `config.json` — **not** a raw absolute path:

```json
{ "modelPath": "@user/my-model.onnx" }
```

Each alias resolves to wherever that kind of asset lives in the environment your extension is
running in, so the same config keeps working as it moves between environments.

### Resolving a URI in C++

**Your extension has to resolve the URI itself.** Nothing expands `@user/...` on the way into
your node, so a URI handed straight to a third-party library (ONNX Runtime, a decoder, a plain
`std::ifstream`) will fail to open. Resolve it first, exactly as the SDK's own `AudioPlayerNode`
and `RecorderNode` do:

```cpp
#include <switchboard/Switchboard.hpp>

std::string resolved = modelPath;   // e.g. "@user/my-model.onnx"
const auto result = Switchboard::callAction("switchboard", "resolveFilePath", { { "path", resolved } });
if (result.isSuccess()) {
    resolved = SBAny::convert<std::string>(result.value());
}
// `resolved` is now an absolute filesystem path; a plain path passes through unchanged,
// so calling this unconditionally is safe.
```

### Don't open an asset in `initialize`

`initialize` runs the moment your extension is loaded, before anyone has had a chance to put a
file in place. An extension that loads its model there fails every time it lands in a fresh
environment. **Store the path in `initialize` and resolve + open it later** — in the node
constructor at the earliest, and better still in `setBusFormat` or on graph start, which leaves
room for the file to arrive and lets the path be changed as a property in between.

The order that works is: load the extension, put the asset in place, *then* add your node and
start the graph. `config.json` may name a file that does not exist yet — it is only strings —
but the file has to be there before anything opens it.

A missing or unresolved asset usually surfaces as a **failing `addNode`** rather than a load
error, because most nodes open their model in the constructor.

---

## 📄 License

This project is licensed under the terms outlined in [`LICENSE.txt`](./LICENSE.txt).

---

## 📬 Contact Us

Need help or have questions?

- 📧 Email: [hello@synervoz.com](mailto:hello@synervoz.com)  
- 📚 Docs: [https://docs.switchboard.audio](https://docs.switchboard.audio)

Happy building! 🎧🚀
