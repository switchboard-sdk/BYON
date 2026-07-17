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

Each node includes two demo projects located in the `demos/` directory:

- **Real-time demo**: Uses `RealtimeGraphRenderer` to process audio from the system mic and play it through the speakers.
- **Offline demo**: Uses `OfflineGraphRenderer` to process audio files and generate output files.

We recommend running these demos to familiarize yourself with how nodes integrate into the SDK.

---

## 🏗 Building the Extension

Use the platform-specific `build` command to compile the extension:

For example, on Linux:

```bash
inv build-linux
```

Additional build commands are available for other platforms. Refer to `tasks.py` for the full list.

---

## 📄 License

This project is licensed under the terms outlined in [`LICENSE.txt`](./LICENSE.txt).

---

## 📬 Contact Us

Need help or have questions?

- 📧 Email: [hello@synervoz.com](mailto:hello@synervoz.com)  
- 📚 Docs: [https://docs.switchboard.audio](https://docs.switchboard.audio)

Happy building! 🎧🚀
