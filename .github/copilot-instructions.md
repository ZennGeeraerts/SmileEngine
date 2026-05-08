# SmileEngine Developer Guidelines

## Architecture Overview

SmileEngine is a modular C++20 game engine for Windows. It is **not a monorepo with a single build** — every project (editor, tests, game) is an independent CMake root that links against its own compiled `smile` static library and picks exactly the modules it needs.

### Repositories / Projects

| Path                                          | Purpose                                                                                                                              |
| --------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| `smile/src/`                                  | Engine static library (`smile`). All module source lives here.                                                                       |
| `smiledit/`                                   | Editor application. Independent CMake root.                                                                                          |
| `smile/tests/unit-tests/`                     | Unit test runner. Independent CMake root.                                                                                            |
| `smile/tests/smile-project/`                  | Visual sample project. Independent CMake root.                                                                                       |
| `smile/tests/sandbox-project/assets/scripts/` | C# sandbox scripts. Independent CMake root.                                                                                          |
| `smile/src/tools/x-shader-c/`                 | Build-time HLSL compiler + reflection tool. Independent CMake root. Produces `.smshader` files consumed by the engine at build time. |
| `smile-script-core/`                          | C# core scripting library (`smile-script-core.dll`). Included via the `scripting` module.                                            |

### Engine Modules (`smile/src/smile/`)

Modules are additive — each `add_module(name)` call in a project's CMakeLists.txt compiles that module's sources into the `smile` static lib.

| Module      | Required | Key systems                                                                                                                                                 |
| ----------- | -------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `common`    | **Yes**  | Primitive types (`String`, `Vector`, `HashMap`, `Handle`), memory (`Ref`, `Scope`), logging, RTTI, type IDs, assertions, platform detection                 |
| `core`      | **Yes**  | `Application`, `Layer`/`LayerStack`, ECS (`ECSEngine`, `System<T>`), `World`/`Entity`, asset management, input, math, serializer, YAML, window/event system |
| `graphic`   | Optional | RHI abstraction (`GraphicsDevice`, `CommandList`), forward renderer, 2D renderer, ImGui integration, mesh/material/scene, shader pipeline                   |
| `physics`   | Optional | PhysX wrapper — `PhysicsEngine`, `PhysicsWorld`, rigid bodies, shapes, character controller                                                                 |
| `scripting` | Optional | Mono/C# scripting — `ScriptEngine`, `ScriptClass`, `ScriptInstance`, glue layer to C++                                                                      |
| `blueprint` | Optional | Visual scripting (interactive graph)                                                                                                                        |

### Platform Backends (`smile/src/platform/`)

Activated via `add_platform(name)` in a project's CMakeLists.txt.

| Backend     | Provides                                                                    |
| ----------- | --------------------------------------------------------------------------- |
| `directx11` | DirectX 11 implementation of the RHI (`GraphicsDevice`, resources, shaders) |
| `physx`     | NVIDIA PhysX implementation of the physics abstraction                      |
| `windows`   | Win32 window, input, and platform layer                                     |

## Build System

- **CMake 3.25+** with **MSVC v143** (Visual Studio 17 2022), C++20.
- Dependencies managed via **Conan 2** — packages output to `conan/Debug/` and `conan/Release/`. Run `scripts/fetch-packages-debug.bat` / `scripts/fetch-packages-release.bat` before first configure.
- Each project has its own `CMakePresets.json` with `x64-Debug` and `x64-Release` presets. Build output goes to `<project>/out/build/<preset>/`.
- **Never add a root `CMakeLists.txt`** — projects are configured and built independently.
- The `smile.cmake` macro file (`smile/src/smile.cmake`) defines `add_module()`, `add_platform()`, and `get_files_recursively()`.
- `x-shader-c` is a build-time tool invoked via `add_custom_command` at configure time by the `graphic` module's `build/compile_shaders.cmake`. It compiles HLSL to DXBC and reflects constant buffer layouts into `.smshader` (custom binary format, magic `0x53484452`).

## Code Conventions

- **Namespace**: everything in `namespace smile`. Sub-namespaces follow module/subsystem (e.g. `smile::ecs`, `smile::graphic`, `smile::application`, `smile::foundation`).
- **Smart pointers**: use engine aliases — `smile::Ref<T>` (`shared_ptr`), `smile::Scope<T>` (`unique_ptr`), created with `CreateRef<T>()` / `CreateScope<T>()`.
- **RTTI**: use the custom macro-based RTTI, not `dynamic_cast`. Declare with `RTTI_DEFINE_BASE(MyClass)` on base, `RTTI_DEFINE(MyClass, BaseClass)` on derived.
- **Assertions**: `SM_ASSERT(cond)`, `SM_ASSERT_MSG(cond, msg)` — active in `SM_C_DEBUG` builds only.
- **Logging**: engine logger via `smile::logging`. Use the logger registry.
- **Macros**: `BIT(x)`, `SM_BIND_EVENT_FN(fn)`, `SAFE_DELETE(p)`, `SAFE_RELEASE(r)`, `SM_DEBUGBREAK()`.
- **Precompiled header**: `smpch.h` is force-included in all engine TUs. Do not include STL headers directly in `.h` files when `smpch.h` already covers them.
- **File header**: every file starts with the standard copyright block.
- **Windows only**: the engine asserts at compile time (`#error`) on non-Windows platforms.
- **ECS systems**: inherit `smile::ecs::System<Derived>` (CRTP). Override `OnAdd`, `OnRemove`, and tick methods.
- **Serializer**: use `generate_serializer_file(target)` in CMakeLists for projects that need serialization; the core module generates `external_serialize_function.cpp` at configure time.

## Key Entry Points

- `smile/src/smile.h` — public include for engine applications.
- `smile/src/smpch.h` — precompiled header.
- `smile/src/smile/core/application/entry_point.h` — defines `main()`, calls the user-provided `CreateApplication()`.
- `smiledit/src/smile_editor_layer.h` — editor layer, the top-level ImGui application layer.

## C# Scripting

- `smile-script-core/Source/Smile/` — engine-side C# API (components, math, input, physics bindings via `InternalCalls.cs`).
- `smile-script-core/Source/Player.cs` — example player script.
- The `scripting` module links Mono statically (`libmono-static-sgen.lib`) from `smile/thirdparty/mono/`.
- Script DLLs are output to `smile/resources/scripts/`.

## VSCode Setup

- `.vscode/settings.json` lists all independent CMake roots under `cmake.sourceDirectory`.
- `.vscode/launch.json` has Debug/Release launch configurations for `smiledit`, `unit-tests`, and `smile-project`.
- Switch the active project in the CMake Tools status bar before building or launching.
- Select `x64-Debug` or `x64-Release` preset from the status bar after switching projects.
