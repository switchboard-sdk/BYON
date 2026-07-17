# Switchboard SDK – C++ Extension Template

[![Build](https://github.com/switchboard-sdk/cpp-extension-template/actions/workflows/build.yml/badge.svg)](https://github.com/switchboard-sdk/cpp-extension-template/actions/workflows/build.yml)

This repository is a template for building custom C++ extensions for the Switchboard SDK. The actual project template lives in [`template/`](./template) — it's example code named `ExampleDSP` that demonstrates the extension architecture (source/processor/sink nodes, demos, CMake setup, etc.).

## 🚀 Generating Your Project

Don't edit `template/` in place. Instead, generate a new, independently-named project directory from it:

```bash
inv rename
```

or run the script directly:

```bash
scripts/rename.sh MyExtensionName [/optional/destination/path]
```

This copies `template/` into a new project directory (a sibling directory named after your project by default), renames all `ExampleDSP`/`EXAMPLEDSP`/`exampledsp` references and file names within the copy, and initializes it with its own fresh git history. The original `template/` directory here is left untouched, so you can run this as many times as you like.

See [`template/README.md`](./template/README.md) for what's inside the generated project and how to build it.
