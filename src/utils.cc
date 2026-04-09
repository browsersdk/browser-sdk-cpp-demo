#include "stdafx.h"

std::string Utils::ReadFile(const stl::path &file, const int &mode) {
  std::string result;
  std::fstream of(file, static_cast<std::ios_base::openmode>(mode));
  if (!of.is_open())
    return result;
  of.seekg(0, of.end);
  const size_t size = static_cast<size_t>(of.tellg());
  if (size > 0) {
    result.resize(size, 0x00);
    of.seekg(0, of.beg);
    of.read(&result[0], static_cast<long>(size));
  }
  of.close();
  return result;
}

uint64_t Utils::TimeStamp() {
  const std::chrono::system_clock::time_point now =
      std::chrono::system_clock::now();
  const std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  const std::chrono::milliseconds ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          now.time_since_epoch()) %
      1000;
  return static_cast<uint64_t>(now_time) * 1000 + ms.count();
}

std::string Utils::Time() {
  const std::chrono::system_clock::time_point now =
      std::chrono::system_clock::now();
  const std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  const std::chrono::milliseconds ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          now.time_since_epoch()) %
      1000;
  std::tm local_time {};
#if defined(__IS_WIN__)
  localtime_s(&local_time, &now_time);
#else
  localtime_r(&now_time, &local_time);
#endif
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
  const DWORD len =
      GetModuleFileNameW(NULL, &tmp[0], static_cast<DWORD>(tmp.size()));
  tmp.resize(len);
  result = tmp;
#elif defined(__IS_LINUX__)
  char path_buf[4096] = {0};
  const ssize_t readlink_len =
      readlink("/proc/self/exe", path_buf, sizeof(path_buf) - 1);
  if (readlink_len > 0) {
    std::string strPath(path_buf, readlink_len);
    result = strPath;
  }
#elif defined(__IS_MAC__)
  char exe_path[PATH_MAX];
  uint32_t size = sizeof(exe_path);
  if (_NSGetExecutablePath(exe_path, &size) == 0) {
    char resolved[PATH_MAX];
    if (realpath(exe_path, resolved) != nullptr)
      result = std::string(resolved);
    else
      result = std::string(exe_path);
  }
#else
#error "Unsupported platform for GetCurrentProcessDir"
#endif
  return result.parent_path();
}

std::string Utils::JsonToString(const rapidjson::Value &valObj) {
  std::string result;
  rapidjson::StringBuffer jbuffer;
  rapidjson::Writer<rapidjson::StringBuffer> jwriter(jbuffer);
  if (valObj.Accept(jwriter))
    result = std::string(jbuffer.GetString(), jbuffer.GetLength());
  return result;
}

std::string Utils::PrettyJson(const std::string &json) {
  rapidjson::Document document;
  if (document.Parse(json.data(), json.size()).HasParseError())
    return json;
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  writer.SetIndent(' ', 2);
  if (!document.Accept(writer))
    return json;
  return std::string(buffer.GetString(), buffer.GetLength());
}

std::vector<std::string> Utils::SplitCommandLine(const std::string &input) {
  std::vector<std::string> tokens;
  std::string current;
  bool in_quotes = false;
  for (size_t i = 0; i < input.size(); ++i) {
    const char ch = input[i];
    if (ch == '\\' && i + 1 < input.size()) {
      const char next = input[i + 1];
      if (next == '"' || next == '\\') {
        current.push_back(next);
        ++i;
        continue;
      }
    }
    if (ch == '"') {
      in_quotes = !in_quotes;
      continue;
    }
    if (!in_quotes && std::isspace(static_cast<unsigned char>(ch))) {
      if (!current.empty()) {
        tokens.push_back(current);
        current.clear();
      }
      continue;
    }
    current.push_back(ch);
  }
  if (!current.empty())
    tokens.push_back(current);
  return tokens;
}

bool Utils::RemoveDir(const stl::path &dir) {
  bool result = false;
  try {
    do {
      if (!std::filesystem::is_directory(dir))
        break;
      result = std::filesystem::remove_all(dir) > 0;
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
    if (SHGetSpecialFolderLocation(nullptr, CSIDL_LOCAL_APPDATA, &ppidl) !=
        S_OK) {
      break;
    }
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
        ((stl::path(homeDir) / "Library/Application Support")).lexically_normal();
  } while (0);
#elif defined(__IS_LINUX__)
  do {
    if (const char *xdg = std::getenv("XDG_DATA_HOME"); xdg && *xdg) {
      result = stl::path(xdg).lexically_normal();
      break;
    }
    char *homeDir = std::getenv("HOME");
    if ((!homeDir || !*homeDir) && ::getuid() != static_cast<uid_t>(-1)) {
      if (passwd *pwd = ::getpwuid(::getuid()); pwd && pwd->pw_dir)
        homeDir = pwd->pw_dir;
    }
    if (!homeDir || !*homeDir)
      break;
    result = ((stl::path(homeDir) / ".local" / "share")).lexically_normal();
  } while (0);
#endif
  return result;
}

bool Utils::StringToBool(const std::string &input, bool *value) {
  if (!value)
    return false;
  const std::string lowered = ToLowerCopy(input);
  if (lowered == "1" || lowered == "true" || lowered == "yes" ||
      lowered == "on") {
    *value = true;
    return true;
  }
  if (lowered == "0" || lowered == "false" || lowered == "no" ||
      lowered == "off") {
    *value = false;
    return true;
  }
  return false;
}

bool Utils::TryParseInt(const std::string &input, int *value) {
  if (!value)
    return false;
  try {
    size_t pos = 0;
    const int parsed = std::stoi(input, &pos, 10);
    if (pos != input.size())
      return false;
    *value = parsed;
    return true;
  } catch (...) {
    return false;
  }
}

std::string Utils::ToLowerCopy(std::string input) {
  std::transform(input.begin(), input.end(), input.begin(),
                 [](unsigned char ch) {
                   return static_cast<char>(std::tolower(ch));
                 });
  return input;
}
