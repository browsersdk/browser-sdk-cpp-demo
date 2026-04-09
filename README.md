# BroSDK C++ Demo

`browser-sdk-cpp-demo` is a standalone native C++ reference project for third-party integrators.

It demonstrates:

- loading `brosdk.dll` / `brosdk.so` / `brosdk.dylib` dynamically
- registering the global async callback before initialization
- calling the main native APIs exposed by `brosdk.h`
- distinguishing async task acceptance from final browser lifecycle events
- using JSON request files for advanced `open / close / env-*` workflows

This directory is designed to be buildable on its own and does not depend on the rest of the NeoFlow build system.

## Directory Layout

```text
browser-sdk-cpp-demo/
├─ 3rdparty/
│  ├─ brosdk/
│  │  ├─ brosdk.h
│  │  └─ brosdk.dll / brosdk.so / brosdk.dylib
│  ├─ dlfcn-win32/
│  └─ rapidjson/
├─ examples/
│  ├─ init.json
│  ├─ open.json
│  ├─ close.json
│  ├─ token-update.json
│  └─ env-page.json
├─ src/
├─ CMakeLists.txt
└─ README.md
```

## What This Demo Covers

- `sdk_init`
- `sdk_info`
- `sdk_browser_info`
- `sdk_browser_install`
- `sdk_browser_open`
- `sdk_browser_close`
- `sdk_token_update`
- `sdk_env_create`
- `sdk_env_update`
- `sdk_env_page`
- `sdk_env_destroy`
- `sdk_shutdown`

## Runtime Search Order

At startup, the demo searches the BroSDK runtime in this order:

1. `BROSDK_RUNTIME` environment variable
2. next to the demo executable
3. `./3rdparty/brosdk/` next to the demo executable
4. system dynamic-library search path

Recommended layout:

- Windows: place `brosdk.dll` in `3rdparty/brosdk/`
- Linux: place `brosdk.so` in `3rdparty/brosdk/`
- macOS: place `brosdk.dylib` in `3rdparty/brosdk/`

The CMake build will automatically copy the runtime into the output directory when the file already exists under `3rdparty/brosdk/`.

## Build

### Windows

```powershell
cmake -S tests/browser-sdk-cpp-demo -B out/browser-sdk-cpp-demo
cmake --build out/browser-sdk-cpp-demo --config Debug
```

### macOS / Linux

```bash
cmake -S tests/browser-sdk-cpp-demo -B out/browser-sdk-cpp-demo
cmake --build out/browser-sdk-cpp-demo
```

## Run

### Interactive Mode

Interactive mode is recommended because `sdk_init`, `open`, `close`, `wait-open`, `wait-close`, and `shutdown` all happen in the same process.

Windows:

```powershell
.\out\browser-sdk-cpp-demo\Debug\browser-sdk-cpp-demo.exe
```

macOS / Linux:

```bash
./out/browser-sdk-cpp-demo/browser-sdk-cpp-demo
```

### One-Shot Mode

You can also execute a single command directly:

```powershell
.\out\browser-sdk-cpp-demo\Debug\browser-sdk-cpp-demo.exe help
.\out\browser-sdk-cpp-demo\Debug\browser-sdk-cpp-demo.exe init-file tests/browser-sdk-cpp-demo/examples/init.json
```

Note:

- JSON file paths are resolved relative to your current working directory
- if you launch the executable from the repository root, prefer `tests/browser-sdk-cpp-demo/examples/...`

## Quick Start

### 1. Initialize the SDK

Option A: pass parameters inline

```text
init <userSig> [workDir] [port] [debug]
```

Example:

```text
init eyJhbGciOi... C:/BroSDK-Demo 9527 true
```

Option B: use a JSON file

```text
init-file tests/browser-sdk-cpp-demo/examples/init.json
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

### 2. Query Runtime Information

```text
info
browser-info
```

### 3. Open a Browser Environment

Simple form:

```text
open 2041695386304778240 https://example.com
```

Advanced form:

```text
open-file tests/browser-sdk-cpp-demo/examples/open.json
```

Example file:

```json
{
  "envs": [
    {
      "envId": "2041695386304778240",
      "urls": ["https://example.com"],
      "args": ["--no-first-run", "--no-default-browser-check"]
    }
  ]
}
```

Wait for the final open result:

```text
wait-open 2041695386304778240 120
```

Important:

- `sdk_browser_open` is asynchronous
- the immediate return code only means the task was accepted or rejected
- the true ready signal is `browser-open-success`
- when `browser-open-success` arrives, CDP is already ready for follow-up business logic

### 4. Close a Browser Environment

Simple form:

```text
close 2041695386304778240
```

Advanced form:

```text
close-file tests/browser-sdk-cpp-demo/examples/close.json
```

Example file:

```json
{
  "envs": ["2041695386304778240"]
}
```

Wait for the final close result:

```text
wait-close 2041695386304778240 120
```

Important:

- `sdk_browser_close` is asynchronous
- the direct return code only means the close task was accepted
- the actual completion signal is `browser-close-success`

### 5. Update User Token

Inline:

```text
token-update <newUserSig>
```

File:

```text
token-update-file tests/browser-sdk-cpp-demo/examples/token-update.json
```

### 6. Shutdown

```text
shutdown
```

## Command Reference

| Command | Description |
| --- | --- |
| `help` | Print command help |
| `init <userSig> [workDir] [port] [debug]` | Call `sdk_init` with inline parameters |
| `init-file <json-file>` | Call `sdk_init` with a JSON request body |
| `info` | Call `sdk_info` |
| `browser-info` | Call `sdk_browser_info` |
| `browser-install-file <json-file>` | Call `sdk_browser_install` |
| `open <envId> [url1] [url2] ...` | Call `sdk_browser_open` with a simple generated body |
| `open-file <json-file>` | Call `sdk_browser_open` with a full JSON body |
| `close <envId> [envId] ...` | Call `sdk_browser_close` with a simple generated body |
| `close-file <json-file>` | Call `sdk_browser_close` with a full JSON body |
| `env-create-file <json-file>` | Call `sdk_env_create` |
| `env-update-file <json-file>` | Call `sdk_env_update` |
| `env-page-file <json-file>` | Call `sdk_env_page` |
| `env-destroy-file <json-file>` | Call `sdk_env_destroy` |
| `token-update <userSig>` | Call `sdk_token_update` |
| `token-update-file <json-file>` | Call `sdk_token_update` with a JSON body |
| `wait-open <envId> [timeoutSeconds]` | Wait for `browser-open-success` or `browser-open-failed` |
| `wait-close <envId> [timeoutSeconds]` | Wait for `browser-close-success` or `browser-close-failed` |
| `events` | Print cached async event summaries |
| `clear-events` | Clear cached async events |
| `shutdown` | Call `sdk_shutdown` |
| `quit` / `exit` / `q` | Exit the demo |

## Async Callback Semantics

This demo registers `sdk_register_result_cb()` immediately after the runtime is loaded.

Every async event is printed to the console and cached in memory.

Current behavior:

- `sdk_init` sync result comes back directly from `sdk_init`
- `sdk_browser_open` final result comes from the async callback
- `sdk_browser_close` final result comes from the async callback
- `sdk_token_update` final result comes from the async callback

Typical browser-open success payload:

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

Interpretation:

- `progress: 100` means startup flow is complete
- `type: browser-open-success` means the environment is ready
- `cdpReady: true` means CDP has already connected successfully

Typical browser-close success payload:

```json
{
  "code": 0,
  "reqId": 1107807335,
  "type": "browser-close-success",
  "msg": "ok",
  "data": {
    "envId": "2041415694746128384",
    "status": 4,
    "statusName": "Stopped",
    "progress": 100,
    "cdpReady": false
  }
}
```

## Environment API Notes

`env-create-file`, `env-update-file`, `env-page-file`, and `env-destroy-file` forward your JSON directly to the backend environment APIs.

That means:

- these commands are synchronous
- the returned body is the raw backend JSON
- the full request-field contract follows the BroSDK backend environment model

The demo includes `examples/env-page.json` as a minimal sample:

```json
{
  "pageIndex": 1,
  "pageSize": 10
}
```

For the full request/response contract, refer to:

- [sdk-reference.md](../../docs/sdk-reference.md)

## Troubleshooting

### Runtime fails to load

Check one of these:

- `BROSDK_RUNTIME` points to the correct file
- the runtime exists next to the executable
- the runtime exists in `3rdparty/brosdk/`

### `sdk_browser_open` or `sdk_browser_close` only returns `1`

That is expected.

- `1` means `CL_DONE`
- it only means the async task was accepted
- the final result still depends on the callback event

### `wait-open` or `wait-close` times out

Check:

- whether `envId` is correct
- whether the SDK was initialized successfully
- whether the async callback already printed a `*-failed` event

## Related Files

- public SDK reference: [sdk-reference.md](../../docs/sdk-reference.md)
- public native header: [brosdk.h](./3rdparty/brosdk/brosdk.h)
