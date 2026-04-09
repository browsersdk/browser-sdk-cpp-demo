#if !defined(__SDK_STDAFX_H__)
#define __SDK_STDAFX_H__

#if defined(_WIN32) || defined(_WIN64)
#define __IS_WIN__
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <shlobj.h>
#include "3rdparty/dlfcn-win32/src/dlfcn.h"
#elif defined(__linux__)
#define __IS_LINUX__
#include <dlfcn.h>
#include <limits.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#elif defined(__APPLE__)
#define __IS_MAC__
#include <dlfcn.h>
#include <limits.h>
#include <mach-o/dyld.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "3rdparty/rapidjson/include/rapidjson/document.h"
#include "3rdparty/rapidjson/include/rapidjson/prettywriter.h"
#include "3rdparty/rapidjson/include/rapidjson/rapidjson.h"
#include "3rdparty/rapidjson/include/rapidjson/stringbuffer.h"
#include "3rdparty/rapidjson/include/rapidjson/writer.h"

namespace stl {
using path = std::filesystem::path;
}

#include "3rdparty/brosdk/brosdk.h"
#include "utils.h"
#include "sdk_wrapper.h"

#endif //__SDK_STDAFX_H__
