# Switchboard SDK – C++ Extension Template

[![Build](https://github.com/switchboard-sdk/cpp-extension-template/actions/workflows/build.yml/badge.svg)](https://github.com/switchboard-sdk/cpp-extension-template/actions/workflows/build.yml)

Welcome to the **Switchboard SDK C++ Extension Template**! This repository provides a streamlined starting point for developers building custom C++ extensions for the Switchboard SDK. With this template, you can set up your development environment quickly and start implementing your own audio processing nodes.

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

## 📄 License

This project is licensed under the terms outlined in [`LICENSE.txt`](./LICENSE.txt).

---

## 📬 Contact Us

Need help or have questions?

- 📧 Email: [hello@synervoz.com](mailto:hello@synervoz.com)  
- 📚 Docs: [https://docs.switchboard.audio](https://docs.switchboard.audio)

Happy building! 🎧🚀
