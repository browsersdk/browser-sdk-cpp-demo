#if !defined(__SDK_DEMO_MAIN_H__)
#define __SDK_DEMO_MAIN_H__

#if defined(_WIN32) || defined(_WIN64)
#define IS_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <shlobj.h>
#include "3rdparty/dlfcn-win32/src/dlfcn.h"
#elif defined(__linux__)
#define IS_LINUX
#include <arpa/inet.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h> // file locks
#include <netinet/in.h>
#include <pwd.h>      // getpwuid
#include <signal.h>   // kill
#include <spawn.h>    // posix_spawn
#include <sys/file.h> // flock
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/wait.h> // waitpid
#include <unistd.h>
#elif defined(__APPLE__)
#define IS_APPLE
#include <netinet/in.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include <spawn.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <libproc.h>
#include <mach-o/dyld.h> // _NSGetExecutablePath
#include <pwd.h>      // getpwuid
#include <Security/Security.h> //coolies
// #include <Cocoa/Cocoa.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>
#include <ImageIO/ImageIO.h>
#endif
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <set>
#include<vector>
#include <random>

#include "brosdk/brosdk.h"
#include "3rdparty/rapidjson/include/rapidjson/document.h"
#include "3rdparty/rapidjson/include/rapidjson/rapidjson.h"
#include "3rdparty/rapidjson/include/rapidjson/reader.h"
#include "3rdparty/rapidjson/include/rapidjson/stream.h"
#include "3rdparty/rapidjson/include/rapidjson/stringbuffer.h"
#include "3rdparty/rapidjson/include/rapidjson/writer.h"
#include "utils.h"
#include "sdk_wrapper.h"

#endif //__SDK_DEMO_MAIN_H__