# BroSDK C++ Demo

[English](README_EN.md) | 简体中文

[![CMake](https://img.shields.io/badge/cmake-%3E%3D3.16-blue.svg)](https://cmake.org/)
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

`browser-cpp-demo` 是 BroSDK 原生 C/C++ SDK 的独立 C++ 示例工程。它演示如何动态加载 `brosdk.dll` / `brosdk.so` / `brosdk.dylib`，注册全局异步回调，并调用 `brosdk.h` 暴露的主要原生 API。

该项目适合需要在 C++ 桌面客户端、自动化工具、测试程序或内部系统中直接接入 BroSDK 的开发者参考。

## 功能定位

- 演示跨平台动态加载 BroSDK 原生运行时。
- 演示在初始化前注册 `sdk_register_result_cb` 全局异步回调。
- 演示 `sdk_init`、`sdk_info`、`sdk_browser_open`、`sdk_browser_close`、`sdk_env_*`、`sdk_token_update` 和 `sdk_shutdown` 等 API。
- 演示如何区分异步任务受理结果与浏览器生命周期最终事件。
- 支持交互式命令行和单命令执行两种运行方式。
- 使用 JSON 文件承载复杂的初始化、启动、关闭和环境管理请求。

## 目录结构

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

## 环境要求

| 项目 | 要求 |
|------|------|
| CMake | 3.16+ |
| C++ | C++20 |
| Windows | Visual Studio 2019+ 或兼容 MSVC 工具链 |
| macOS / Linux | 支持 C++20 的 Clang 或 GCC |
| BroSDK 原生库 | `brosdk.dll`、`brosdk.so` 或 `brosdk.dylib` |

Windows 下项目内置 `dlfcn-win32` 用于兼容 `dlopen` / `dlsym` 风格的动态加载。

## 原生库放置

运行时会按以下顺序查找 BroSDK 原生库：

1. `BROSDK_RUNTIME` 环境变量指向的完整文件路径。
2. Demo 可执行文件同级目录。
3. Demo 可执行文件同级目录下的 `3rdparty/brosdk/`。
4. 系统动态库搜索路径。

推荐放置方式：

| 平台 | 文件 |
|------|------|
| Windows | `3rdparty/brosdk/brosdk.dll` |
| Linux | `3rdparty/brosdk/brosdk.so` |
| macOS | `3rdparty/brosdk/brosdk.dylib` |

如果对应文件已存在于 `3rdparty/brosdk/`，CMake 构建后会自动复制到可执行文件输出目录。

## 构建

### Windows

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

运行产物通常位于：

```text
build/Debug/browser-sdk-cpp-demo.exe
```

### macOS / Linux

```bash
cmake -S . -B build
cmake --build build
```

运行产物通常位于：

```text
build/browser-sdk-cpp-demo
```

## 快速开始

### 1. 启动交互式模式

Windows：

```powershell
.\build\Debug\browser-sdk-cpp-demo.exe
```

macOS / Linux：

```bash
./build/browser-sdk-cpp-demo
```

交互式模式推荐用于完整调试，因为 `sdk_init`、`open`、`wait-open`、`close`、`wait-close` 和 `shutdown` 都在同一个进程内执行。

### 2. 初始化 SDK

使用内联参数：

```text
init <userSig> [workDir] [port] [debug]
```

示例：

```text
init eyJhbGciOi... C:/BroSDK-Demo 9527 true
```

使用 JSON 文件：

```text
init-file examples/init.json
```

示例文件：

```json
{
  "userSig": "REPLACE_WITH_YOUR_USER_SIG",
  "workDir": "C:/BroSDK-Demo",
  "port": 9527,
  "debug": true
}
```

### 3. 查询运行时信息

```text
info
browser-info
```

### 4. 启动浏览器环境

简单命令：

```text
open 2041695386304778240 https://example.com
```

使用 JSON 文件：

```text
open-file examples/open.json
```

等待最终启动结果：

```text
wait-open 2041695386304778240 120
```

`sdk_browser_open` 是异步接口。直接返回码只表示任务是否被受理，最终结果应以异步回调中的 `browser-open-success` 或 `browser-open-failed` 为准。收到 `browser-open-success` 后，CDP 通常已经可用于后续业务逻辑。

### 5. 关闭浏览器环境

简单命令：

```text
close 2041695386304778240
```

使用 JSON 文件：

```text
close-file examples/close.json
```

等待最终关闭结果：

```text
wait-close 2041695386304778240 120
```

`sdk_browser_close` 也是异步接口，最终结果以 `browser-close-success` 或 `browser-close-failed` 回调事件为准。

### 6. 更新 Token

```text
token-update <newUserSig>
token-update-file examples/token-update.json
```

### 7. 关闭 SDK

```text
shutdown
```

## 单命令模式

也可以直接执行单个命令：

```powershell
.\build\Debug\browser-sdk-cpp-demo.exe help
.\build\Debug\browser-sdk-cpp-demo.exe init-file examples/init.json
```

注意：JSON 文件路径按当前工作目录解析。如果从仓库根目录运行，直接使用 `examples/...`。

## 命令参考

| 命令 | 说明 |
|------|------|
| `help` | 输出命令帮助 |
| `init <userSig> [workDir] [port] [debug]` | 使用内联参数调用 `sdk_init` |
| `init-file <json-file>` | 使用 JSON 请求体调用 `sdk_init` |
| `info` | 调用 `sdk_info` |
| `browser-info` | 调用 `sdk_browser_info` |
| `browser-install-file <json-file>` | 调用 `sdk_browser_install` |
| `open <envId> [url1] [url2] ...` | 生成简单请求并调用 `sdk_browser_open` |
| `open-file <json-file>` | 使用完整 JSON 请求体调用 `sdk_browser_open` |
| `close <envId> [envId] ...` | 生成简单请求并调用 `sdk_browser_close` |
| `close-file <json-file>` | 使用完整 JSON 请求体调用 `sdk_browser_close` |
| `env-create-file <json-file>` | 调用 `sdk_env_create` |
| `env-update-file <json-file>` | 调用 `sdk_env_update` |
| `env-page-file <json-file>` | 调用 `sdk_env_page` |
| `env-destroy-file <json-file>` | 调用 `sdk_env_destroy` |
| `token-update <userSig>` | 调用 `sdk_token_update` |
| `token-update-file <json-file>` | 使用 JSON 请求体调用 `sdk_token_update` |
| `wait-open <envId> [timeoutSeconds]` | 等待 `browser-open-success` 或 `browser-open-failed` |
| `wait-close <envId> [timeoutSeconds]` | 等待 `browser-close-success` 或 `browser-close-failed` |
| `events` | 输出已缓存的异步事件摘要 |
| `clear-events` | 清空已缓存的异步事件 |
| `shutdown` | 调用 `sdk_shutdown` |
| `quit` / `exit` / `q` | 退出 Demo |

## 异步回调语义

Demo 在加载运行时后立即注册 `sdk_register_result_cb()`。所有异步事件会打印到控制台，并缓存在内存中供 `events`、`wait-open` 和 `wait-close` 使用。

当前行为：

- `sdk_init` 的同步结果由 `sdk_init` 直接返回。
- `sdk_browser_open` 的最终结果来自异步回调。
- `sdk_browser_close` 的最终结果来自异步回调。
- `sdk_token_update` 的最终结果来自异步回调。

典型启动成功事件：

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

字段含义：

- `progress: 100` 表示启动流程完成。
- `type: browser-open-success` 表示环境已就绪。
- `cdpReady: true` 表示 CDP 已连接成功。

## 环境 API 说明

`env-create-file`、`env-update-file`、`env-page-file` 和 `env-destroy-file` 会将 JSON 请求体直接转发给 BroSDK 环境 API。

这意味着：

- 这些命令是同步接口。
- 返回体是原始后端 JSON。
- 完整字段契约以 BroSDK SDK 文档和服务端环境模型为准。

最小分页查询示例：

```json
{
  "pageIndex": 1,
  "pageSize": 10
}
```

## 故障排查

### 原生库加载失败

检查：

- `BROSDK_RUNTIME` 是否指向正确文件。
- 原生库是否位于可执行文件同级目录。
- 原生库是否位于 `3rdparty/brosdk/`。
- 当前平台的动态库文件名是否正确。

### `sdk_browser_open` 或 `sdk_browser_close` 直接返回 `1`

这是预期行为。

- `1` 表示 `CL_DONE`。
- 它只表示异步任务已受理。
- 最终结果仍取决于回调事件。

### `wait-open` 或 `wait-close` 超时

检查：

- `envId` 是否正确。
- SDK 是否初始化成功。
- 控制台是否已经打印 `*-failed` 事件。
- 浏览器内核是否仍在下载或启动中。

## 相关仓库

| 仓库 | 说明 |
|------|------|
| [brosdk](https://github.com/browsersdk/brosdk) | BroSDK 原生 SDK，提供 C/C++ API、头文件和平台动态库 |
| [brosdk-core](https://github.com/browsersdk/brosdk-core) | BroSDK 浏览器指纹内核发布版本 |
| [brosdk-docs](https://github.com/browsersdk/brosdk-docs) | SDK 文档、API 说明和接入指南 |
| [browser-demo](https://github.com/browsersdk/browser-demo) | 完整服务端和桌面客户端示例 |

## License

MIT
