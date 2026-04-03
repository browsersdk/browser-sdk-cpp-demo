#include "stdafx.h"

std::string Utils::ReadFile(const stl::path &file, const int &mode) {
  std::string result;
  std::fstream of(file, static_cast<std::ios_base::openmode>(mode));
  if (!of.is_open())
    return result;
  of.seekg(0, of.end);
  size_t size = static_cast<size_t>(of.tellg());
  if (size > 0) {
    result.resize(size, 0x00);
    of.seekg(0, of.beg);
    of.read(&result[0], static_cast<long>(size));
  }
  of.close();
  return result;
}
uint64_t Utils::TimeStamp() {
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::chrono::milliseconds ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          now.time_since_epoch()) %
      1000;
  return static_cast<uint64_t>(now_time) * 1000 + ms.count();
}
std::string Utils::Time() {
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::chrono::milliseconds ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          now.time_since_epoch()) %
      1000;
  std::tm local_time = *std::localtime(&now_time);
  std::ostringstream oss;
  oss << std::put_time(&local_time, "%Y/%m/%d %H:%M:%S");
  oss << '/' << std::setw(3) << std::setfill('0') << ms.count() << "\t";
  return oss.str();
}
stl::path Utils::GetCurrentProcessDir() {
  stl::path result;
#if defined(__IS_WIN__)
  std::wstring tmp;
  tmp.resize(MAX_PATH, 0x00);
  DWORD len = GetModuleFileNameW(NULL, &tmp[0], static_cast<DWORD>(tmp.size()));
  tmp.resize(len);
  result = tmp;
#elif defined(__IS_LINUX__)
  char path_buf[4096] = {0};
  ssize_t readlink_len =
      readlink("/proc/self/exe", path_buf, sizeof(path_buf) - 1);
  if (readlink_len > 0) {
    std::string strPath(path_buf, readlink_len);
    result = strPath;
  }
#elif defined(__IS_MAC__)
  // macOS: use _NSGetExecutablePath to get the (possibly unresolved) path,
  // then canonicalize with realpath when possible.
  char exe_path[PATH_MAX];
  uint32_t size = sizeof(exe_path);
  if (_NSGetExecutablePath(exe_path, &size) == 0) {
    char resolved[PATH_MAX];
    if (realpath(exe_path, resolved) != nullptr) {
      result = std::string(resolved);
    } else {
      // fall back to the returned path if realpath fails
      result = std::string(exe_path);
    }
  }
#else
#error "Unsupported platform for GetCurrentProcessDir"
#endif
  return result.parent_path();
}

void Utils::MainProc(const std::function<void(const std::string &input,
                                              bool &exit_flag)> &callback_) {
  std::string input;
  do {
    input.clear();
    std::getline(std::cin, input);
    if (input.empty()) {
      continue;
    }
    bool exit_flag = false;
    callback_(input, exit_flag);
    if (exit_flag || std::cin.eof()) {
      break;
    }
  } while (true);
}
std::string Utils::JsonToString(const rapidjson::Value &valObj) {
  std::string result;
  rapidjson::StringBuffer jbuffer;
  rapidjson::Writer<rapidjson::StringBuffer> jwriter(jbuffer);
  if (valObj.Accept(jwriter))
    result = std::string(jbuffer.GetString(), jbuffer.GetLength());
  return result;
}
bool Utils::RemoveDir(const stl::path &dir) {
  bool result = false;
  try {
    do {
      if (!std::filesystem::is_directory(dir))
        break;
      result = std::filesystem::remove_all(dir);
    } while (0);
  } catch (...) {
    result = false;
  }
  return result;
}

stl::path Utils::GetAppDataDir() {
  stl::path result;
#if defined(__IS_WIN__)
  LPITEMIDLIST ppidl = nullptr;
  do {
    if (SHGetSpecialFolderLocation(nullptr, CSIDL_LOCAL_APPDATA,
                                   // /*CSIDL_APPDATA*/  CSIDL_COMMON_APPDATA,
                                   &ppidl) != S_OK)
      break;
    wchar_t pszPath[_MAX_PATH] = {};
    if (!SHGetPathFromIDListW(ppidl, pszPath))
      break;
    result = pszPath;
  } while (0);
  if (ppidl) {
    CoTaskMemFree(ppidl);
    ppidl = nullptr;
  }
#elif defined(__IS_MAC__)
  do {
    char *homeDir = std::getenv("HOME");
    if ((!homeDir || !*homeDir) && ::getuid() != static_cast<uid_t>(-1)) {
      if (passwd *pwd = ::getpwuid(::getuid()); pwd && pwd->pw_dir)
        homeDir = pwd->pw_dir;
    }
    if (!homeDir || !*homeDir)
      break;
    result =
        ((path(homeDir) / "Library/Application Support")).lexically_normal();
  } while (0);
#elif defined(__IS_LINUX__)

#endif
  return result;
}