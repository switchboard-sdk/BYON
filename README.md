# BYON – Switchboard SDK Extension Factory

[![Build](https://github.com/switchboard-sdk/BYON/actions/workflows/build.yml/badge.svg)](https://github.com/switchboard-sdk/BYON/actions/workflows/build.yml)

BYON ("Bring Your Own Node") is an extension **factory** for the Switchboard SDK: it generates a new, independently-named C++ extension project that you then own and develop. It isn't a template you edit in place — you run the generator and it stamps out your project.

What gets stamped out lives in [`template/`](./template) — example code named `ExampleDSP` that demonstrates the extension architecture (source/processor/sink nodes, demos, CMake setup, etc.).

For the concepts behind custom nodes and extensions, see the [Bring Your Own Node](https://docs.switchboard.audio/guide/bring-your-own-node/overview/) guide.

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

See [`template/README.md`](./template/README.md) for what's inside the generated project and how to build it. Once you have a project, that README also covers [adding your own nodes](./template/README.md#-adding-your-own-nodes), [configuring the extension at load time](./template/README.md#configuring-your-extension-at-load-time-configjson), and [shipping models and other assets](./template/README.md#-assets-models-binaries-audio).
