# BroSDK C++ Demo

English | [简体中文](README.md)

[![CMake](https://img.shields.io/badge/cmake-%3E%3D3.16-blue.svg)](https://cmake.org/)
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

`browser-cpp-demo` is a standalone C++ reference project for the BroSDK native C/C++ SDK. It demonstrates how to dynamically load `brosdk.dll` / `brosdk.so` / `brosdk.dylib`, register the global asynchronous callback, and call the main native APIs exposed by `brosdk.h`.

This project is intended for developers who need to integrate BroSDK directly into C++ desktop clients, automation tools, test programs, or internal systems.

## Repository Scope

- Demonstrate cross-platform dynamic loading of the BroSDK native runtime.
- Register the global `sdk_register_result_cb` asynchronous callback before initialization.
- Demonstrate APIs such as `sdk_init`, `sdk_info`, `sdk_browser_open`, `sdk_browser_close`, `sdk_env_*`, `sdk_token_update`, and `sdk_shutdown`.
- Show the difference between asynchronous task acceptance and final browser lifecycle events.
- Support both interactive command-line mode and one-shot command mode.
- Use JSON files for complex initialization, launch, close, and environment management requests.

## Directory Layout

```text
browser-cpp-demo/
├── 3rdparty/
│   ├── brosdk/
│   │   ├── brosdk.h
│   │   └── brosdk.dll / brosdk.so / brosdk.dylib
│   ├── dlfcn-win32/
│   └── rapidjson/
├── examples/
│   ├── init.json
│   ├── open.json
│   ├── close.json
│   ├── token-update.json
│   └── env-page.json
├── src/
│   ├── main.cc
│   ├── sdk_wrapper.h
│   ├── sdk_wrapper.cc
│   ├── utils.h
│   └── utils.cc
├── CMakeLists.txt
├── LICENSE
├── README.md
└── README_EN.md
```

## Requirements

| Item | Requirement |
|------|-------------|
| CMake | 3.16+ |
| C++ | C++20 |
| Windows | Visual Studio 2019+ or a compatible MSVC toolchain |
| macOS / Linux | Clang or GCC with C++20 support |
| BroSDK native library | `brosdk.dll`, `brosdk.so`, or `brosdk.dylib` |

On Windows, this project includes `dlfcn-win32` to provide `dlopen` / `dlsym` style dynamic loading.

## Native Library Placement

At startup, the demo searches for the BroSDK native library in this order:

1. Full path from the `BROSDK_RUNTIME` environment variable.
2. Next to the demo executable.
3. `3rdparty/brosdk/` next to the demo executable.
4. System dynamic-library search path.

Recommended placement:

| Platform | File |
|----------|------|
| Windows | `3rdparty/brosdk/brosdk.dll` |
| Linux | `3rdparty/brosdk/brosdk.so` |
| macOS | `3rdparty/brosdk/brosdk.dylib` |

If the corresponding file already exists under `3rdparty/brosdk/`, the CMake build copies it to the executable output directory after build.

## Build

### Windows

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

The executable is usually generated at:

```text
build/Debug/browser-sdk-cpp-demo.exe
```

### macOS / Linux

```bash
cmake -S . -B build
cmake --build build
```

The executable is usually generated at:

```text
build/browser-sdk-cpp-demo
```

## Quick Start

### 1. Start Interactive Mode

Windows:

```powershell
.\build\Debug\browser-sdk-cpp-demo.exe
```

macOS / Linux:

```bash
./build/browser-sdk-cpp-demo
```

Interactive mode is recommended for full debugging because `sdk_init`, `open`, `wait-open`, `close`, `wait-close`, and `shutdown` all run in the same process.

### 2. Initialize The SDK

Use inline parameters:

```text
init <userSig> [workDir] [port] [debug]
```

Example:

```text
init eyJhbGciOi... C:/BroSDK-Demo 9527 true
```

Use a JSON file:

```text
init-file examples/init.json
```

Example file:

```json
{
  "userSig": "REPLACE_WITH_YOUR_USER_SIG",
  "workDir": "C:/BroSDK-Demo",
  "port": 9527,
  "debug": true
}
```

### 3. Query Runtime Information

```text
info
browser-info
```

### 4. Open A Browser Environment

Simple command:

```text
open 2041695386304778240 https://example.com
```

Use a JSON file:

```text
open-file examples/open.json
```

Wait for the final open result:

```text
wait-open 2041695386304778240 120
```

`sdk_browser_open` is asynchronous. The direct return code only indicates whether the task was accepted. The final result should be determined by `browser-open-success` or `browser-open-failed` from the asynchronous callback. After `browser-open-success`, CDP is usually ready for follow-up business logic.

### 5. Close A Browser Environment

Simple command:

```text
close 2041695386304778240
```

Use a JSON file:

```text
close-file examples/close.json
```

Wait for the final close result:

```text
wait-close 2041695386304778240 120
```

`sdk_browser_close` is also asynchronous. The final result should be determined by `browser-close-success` or `browser-close-failed` callback events.

### 6. Update Token

```text
token-update <newUserSig>
token-update-file examples/token-update.json
```

### 7. Shut Down The SDK

```text
shutdown
```

## One-Shot Mode

You can also execute a single command directly:

```powershell
.\build\Debug\browser-sdk-cpp-demo.exe help
.\build\Debug\browser-sdk-cpp-demo.exe init-file examples/init.json
```

JSON file paths are resolved relative to the current working directory. If you run from the repository root, use `examples/...`.

## Command Reference

| Command | Description |
|---------|-------------|
| `help` | Print command help |
| `init <userSig> [workDir] [port] [debug]` | Call `sdk_init` with inline parameters |
| `init-file <json-file>` | Call `sdk_init` with a JSON request body |
| `info` | Call `sdk_info` |
| `browser-info` | Call `sdk_browser_info` |
| `browser-install-file <json-file>` | Call `sdk_browser_install` |
| `open <envId> [url1] [url2] ...` | Generate a simple request and call `sdk_browser_open` |
| `open-file <json-file>` | Call `sdk_browser_open` with a full JSON body |
| `close <envId> [envId] ...` | Generate a simple request and call `sdk_browser_close` |
| `close-file <json-file>` | Call `sdk_browser_close` with a full JSON body |
| `env-create-file <json-file>` | Call `sdk_env_create` |
| `env-update-file <json-file>` | Call `sdk_env_update` |
| `env-page-file <json-file>` | Call `sdk_env_page` |
| `env-destroy-file <json-file>` | Call `sdk_env_destroy` |
| `token-update <userSig>` | Call `sdk_token_update` |
| `token-update-file <json-file>` | Call `sdk_token_update` with a JSON body |
| `wait-open <envId> [timeoutSeconds]` | Wait for `browser-open-success` or `browser-open-failed` |
| `wait-close <envId> [timeoutSeconds]` | Wait for `browser-close-success` or `browser-close-failed` |
| `events` | Print cached asynchronous event summaries |
| `clear-events` | Clear cached asynchronous events |
| `shutdown` | Call `sdk_shutdown` |
| `quit` / `exit` / `q` | Exit the demo |

## Async Callback Semantics

The demo registers `sdk_register_result_cb()` immediately after the runtime is loaded. All asynchronous events are printed to the console and cached in memory for `events`, `wait-open`, and `wait-close`.

Current behavior:

- `sdk_init` synchronous result is returned directly by `sdk_init`.
- `sdk_browser_open` final result comes from the asynchronous callback.
- `sdk_browser_close` final result comes from the asynchronous callback.
- `sdk_token_update` final result comes from the asynchronous callback.

Typical browser-open success event:

```json
{
  "code": 0,
  "reqId": 369488048,
  "type": "browser-open-success",
  "msg": "ok",
  "data": {
    "envId": "2041695386304778240",
    "status": 2,
    "statusName": "Started",
    "progress": 100,
    "cdpReady": true
  }
}
```

Field interpretation:

- `progress: 100` means the startup flow is complete.
- `type: browser-open-success` means the environment is ready.
- `cdpReady: true` means CDP has connected successfully.

## Environment API Notes

`env-create-file`, `env-update-file`, `env-page-file`, and `env-destroy-file` forward the JSON request body directly to the BroSDK environment APIs.

That means:

- These commands are synchronous.
- The returned body is the raw backend JSON.
- The full field contract follows the BroSDK SDK documentation and server-side environment model.

Minimal page query example:

```json
{
  "pageIndex": 1,
  "pageSize": 10
}
```

## Troubleshooting

### Native library fails to load

Check:

- `BROSDK_RUNTIME` points to the correct file.
- The native library exists next to the executable.
- The native library exists under `3rdparty/brosdk/`.
- The dynamic library filename matches the current platform.

### `sdk_browser_open` or `sdk_browser_close` directly returns `1`

This is expected.

- `1` means `CL_DONE`.
- It only means the asynchronous task was accepted.
- The final result still depends on callback events.

### `wait-open` or `wait-close` times out

Check:

- The `envId` is correct.
- The SDK was initialized successfully.
- The console already printed a `*-failed` event.
- The browser core is still downloading or starting.

## Related Repositories

| Repository | Description |
|------------|-------------|
| [brosdk](https://github.com/browsersdk/brosdk) | Native BroSDK repository with C/C++ APIs, headers, and platform dynamic libraries |
| [brosdk-core](https://github.com/browsersdk/brosdk-core) | BroSDK browser fingerprint core releases |
| [brosdk-docs](https://github.com/browsersdk/brosdk-docs) | SDK documentation, API references, and integration guides |
| [browser-demo](https://github.com/browsersdk/browser-demo) | Full server-side and desktop client example |

## License

MIT
