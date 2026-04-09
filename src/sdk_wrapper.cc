#include "stdafx.h"

namespace {
std::string JsonScalarToString(const rapidjson::Value &value) {
  if (value.IsString())
    return value.GetString();
  if (value.IsUint64())
    return std::to_string(value.GetUint64());
  if (value.IsInt64())
    return std::to_string(value.GetInt64());
  if (value.IsUint())
    return std::to_string(value.GetUint());
  if (value.IsInt())
    return std::to_string(value.GetInt());
  return {};
}

std::string ReadJsonFile(const stl::path &file) {
  if (file.empty() || !std::filesystem::exists(file))
    return {};
  return Utils::ReadFile(file);
}
} // namespace

SDKWrapper::SDKWrapper() {
  LoadModule();
}

SDKWrapper::~SDKWrapper() {
  UnloadModule();
}

bool SDKWrapper::Ready() const {
  return ready_.load();
}

std::string SDKWrapper::LastError() const {
  return last_error_;
}

stl::path SDKWrapper::ModulePath() const {
  return module_path_;
}

std::string SDKWrapper::DescribeCode(int32_t code) const {
  std::ostringstream stream;
  stream << code;
  if (fn_sdk_error_name_) {
    if (const char *name = fn_sdk_error_name_(code); name && *name)
      stream << " " << name;
  }
  if (fn_sdk_error_string_) {
    if (const char *text = fn_sdk_error_string_(code); text && *text)
      stream << " (" << text << ")";
  }
  return stream.str();
}

bool SDKWrapper::LoadModule() {
  current_dir_ = Utils::GetCurrentProcessDir();
#if defined(__IS_WIN__)
  const char *runtime_name = "brosdk.dll";
#elif defined(__IS_MAC__)
  const char *runtime_name = "brosdk.dylib";
#else
  const char *runtime_name = "brosdk.so";
#endif

  std::vector<stl::path> candidates;
  if (const char *runtime = std::getenv("BROSDK_RUNTIME"); runtime && *runtime)
    candidates.emplace_back(runtime);
  candidates.emplace_back((current_dir_ / runtime_name).lexically_normal());
  candidates.emplace_back(
      (current_dir_ / "3rdparty" / "brosdk" / runtime_name).lexically_normal());

  for (const auto &candidate : candidates) {
    if (candidate.empty())
      continue;
    module_ = dlopen(candidate.string().c_str(), RTLD_NOW);
    if (module_) {
      module_path_ = candidate;
      break;
    }
  }

  if (!module_) {
    module_ = dlopen(runtime_name, RTLD_NOW);
    if (module_)
      module_path_ = runtime_name;
  }

  if (!module_) {
    last_error_ = "failed to load BroSDK runtime. Set BROSDK_RUNTIME or place "
                  "brosdk next to the demo executable.";
    return false;
  }

  if (!ResolveFunctions()) {
    UnloadModule();
    return false;
  }

  const int32_t register_code =
      fn_sdk_register_result_cb_(SDKWrapper::SDKResultCallback, this);
  if (register_code != 0) {
    last_error_ = std::string("sdk_register_result_cb failed: ") +
                  DescribeCode(register_code);
    UnloadModule();
    return false;
  }

  ready_.store(true);
  return true;
}

void SDKWrapper::UnloadModule() {
  ready_.store(false);
  events_.clear();

  if (!module_)
    return;

  dlclose(module_);
  module_ = nullptr;
  module_path_.clear();
}

bool SDKWrapper::ResolveFunctions() {
#define RESOLVE_REQUIRED(field, symbol_name)                                    \
  field = reinterpret_cast<decltype(field)>(dlsym(module_, symbol_name));       \
  if (!(field)) {                                                               \
    last_error_ = std::string("missing required symbol: ") + symbol_name;       \
    return false;                                                               \
  }

  RESOLVE_REQUIRED(fn_sdk_register_result_cb_, "sdk_register_result_cb");
  RESOLVE_REQUIRED(fn_sdk_init_, "sdk_init");
  RESOLVE_REQUIRED(fn_sdk_info_, "sdk_info");
  RESOLVE_REQUIRED(fn_sdk_browser_install_, "sdk_browser_install");
  RESOLVE_REQUIRED(fn_sdk_browser_info_, "sdk_browser_info");
  RESOLVE_REQUIRED(fn_sdk_browser_open_, "sdk_browser_open");
  RESOLVE_REQUIRED(fn_sdk_browser_close_, "sdk_browser_close");
  RESOLVE_REQUIRED(fn_sdk_env_create_, "sdk_env_create");
  RESOLVE_REQUIRED(fn_sdk_env_update_, "sdk_env_update");
  RESOLVE_REQUIRED(fn_sdk_env_page_, "sdk_env_page");
  RESOLVE_REQUIRED(fn_sdk_env_destroy_, "sdk_env_destroy");
  RESOLVE_REQUIRED(fn_sdk_token_update_, "sdk_token_update");
  RESOLVE_REQUIRED(fn_sdk_shutdown_, "sdk_shutdown");
  RESOLVE_REQUIRED(fn_sdk_free_, "sdk_free");
  RESOLVE_REQUIRED(fn_sdk_error_name_, "sdk_error_name");
  RESOLVE_REQUIRED(fn_sdk_error_string_, "sdk_error_string");

#undef RESOLVE_REQUIRED
  return true;
}

int32_t SDKWrapper::CallSyncNoArgs(sync_noargs_fn fn, std::string *response) const {
  std::lock_guard<std::mutex> lock(api_mutex_);
  if (!ready_.load() || !fn)
    return -1;
  char *out_data = nullptr;
  size_t out_len = 0;
  const int32_t code = fn(&out_data, &out_len);
  if (response) {
    response->clear();
    if (out_data && out_len > 0)
      response->assign(out_data, out_len);
  }
  if (out_data)
    fn_sdk_free_(out_data);
  return code;
}

int32_t SDKWrapper::CallSyncJson(sync_json_fn fn, const std::string &json,
                                 std::string *response) const {
  std::lock_guard<std::mutex> lock(api_mutex_);
  if (!ready_.load() || !fn)
    return -1;
  char *out_data = nullptr;
  size_t out_len = 0;
  const int32_t code = fn(json.data(), json.size(), &out_data, &out_len);
  if (response) {
    response->clear();
    if (out_data && out_len > 0)
      response->assign(out_data, out_len);
  }
  if (out_data)
    fn_sdk_free_(out_data);
  return code;
}

int32_t SDKWrapper::CallAsyncJson(async_json_fn fn, const std::string &json) const {
  std::lock_guard<std::mutex> lock(api_mutex_);
  if (!ready_.load() || !fn)
    return -1;
  return fn(json.data(), json.size());
}

std::string SDKWrapper::BuildInitJson(const DemoInitOptions &options) const {
  rapidjson::Document document(rapidjson::kObjectType);
  auto &allocator = document.GetAllocator();
  document.AddMember(
      "userSig",
      rapidjson::Value(options.user_sig.c_str(), allocator).Move(), allocator);
  if (!options.work_dir.empty()) {
    document.AddMember(
        "workDir",
        rapidjson::Value(options.work_dir.c_str(), allocator).Move(),
        allocator);
  }
  if (!options.sdk_api_url.empty()) {
    document.AddMember("sdkApiUrl",
                       rapidjson::Value(options.sdk_api_url.c_str(), allocator)
                           .Move(),
                       allocator);
  }
  if (options.port > 0)
    document.AddMember("port", options.port, allocator);
  document.AddMember("debug", options.debug, allocator);
  return Utils::JsonToString(document);
}

std::string SDKWrapper::BuildOpenJson(const DemoBrowserOpenOptions &options) const {
  rapidjson::Document document(rapidjson::kObjectType);
  auto &allocator = document.GetAllocator();
  rapidjson::Value envs(rapidjson::kArrayType);
  rapidjson::Value env(rapidjson::kObjectType);
  env.AddMember(
      "envId", rapidjson::Value(options.env_id.c_str(), allocator).Move(),
      allocator);
  if (!options.urls.empty()) {
    rapidjson::Value urls(rapidjson::kArrayType);
    for (const auto &url : options.urls) {
      urls.PushBack(rapidjson::Value(url.c_str(), allocator).Move(), allocator);
    }
    env.AddMember("urls", urls.Move(), allocator);
  }
  if (!options.args.empty()) {
    rapidjson::Value args(rapidjson::kArrayType);
    for (const auto &arg : options.args) {
      args.PushBack(rapidjson::Value(arg.c_str(), allocator).Move(), allocator);
    }
    env.AddMember("args", args.Move(), allocator);
  }
  envs.PushBack(env.Move(), allocator);
  document.AddMember("envs", envs.Move(), allocator);
  return Utils::JsonToString(document);
}

std::string SDKWrapper::BuildCloseJson(
    const std::vector<std::string> &env_ids) const {
  rapidjson::Document document(rapidjson::kObjectType);
  auto &allocator = document.GetAllocator();
  rapidjson::Value envs(rapidjson::kArrayType);
  for (const auto &env_id : env_ids) {
    envs.PushBack(rapidjson::Value(env_id.c_str(), allocator).Move(),
                  allocator);
  }
  document.AddMember("envs", envs.Move(), allocator);
  return Utils::JsonToString(document);
}

int32_t SDKWrapper::Init(const DemoInitOptions &options, std::string *response) {
  return InitWithJson(BuildInitJson(options), response);
}

int32_t SDKWrapper::InitWithJson(const std::string &json, std::string *response) {
  std::lock_guard<std::mutex> lock(api_mutex_);
  if (!ready_.load() || !fn_sdk_init_)
    return -1;
  char *out_data = nullptr;
  size_t out_len = 0;
  const int32_t code =
      fn_sdk_init_(nullptr, json.data(), json.size(), &out_data, &out_len);
  if (response) {
    response->clear();
    if (out_data && out_len > 0)
      response->assign(out_data, out_len);
  }
  if (out_data)
    fn_sdk_free_(out_data);
  return code;
}

int32_t SDKWrapper::InitWithJsonFile(const stl::path &file,
                                     std::string *response) {
  return InitWithJson(ReadJsonFile(file), response);
}

int32_t SDKWrapper::Info(std::string *response) const {
  return CallSyncNoArgs(fn_sdk_info_, response);
}

int32_t SDKWrapper::BrowserInfo(std::string *response) const {
  return CallSyncNoArgs(fn_sdk_browser_info_, response);
}

int32_t SDKWrapper::BrowserInstallWithJson(const std::string &json) const {
  return CallAsyncJson(fn_sdk_browser_install_, json);
}

int32_t SDKWrapper::BrowserInstallWithJsonFile(const stl::path &file) const {
  return BrowserInstallWithJson(ReadJsonFile(file));
}

int32_t SDKWrapper::BrowserOpen(const DemoBrowserOpenOptions &options) const {
  return BrowserOpenWithJson(BuildOpenJson(options));
}

int32_t SDKWrapper::BrowserOpenWithJson(const std::string &json) const {
  return CallAsyncJson(fn_sdk_browser_open_, json);
}

int32_t SDKWrapper::BrowserOpenWithJsonFile(const stl::path &file) const {
  return BrowserOpenWithJson(ReadJsonFile(file));
}

int32_t SDKWrapper::BrowserClose(const std::vector<std::string> &env_ids) const {
  return BrowserCloseWithJson(BuildCloseJson(env_ids));
}

int32_t SDKWrapper::BrowserCloseWithJson(const std::string &json) const {
  return CallAsyncJson(fn_sdk_browser_close_, json);
}

int32_t SDKWrapper::BrowserCloseWithJsonFile(const stl::path &file) const {
  return BrowserCloseWithJson(ReadJsonFile(file));
}

int32_t SDKWrapper::EnvCreateWithJson(const std::string &json,
                                      std::string *response) const {
  return CallSyncJson(fn_sdk_env_create_, json, response);
}

int32_t SDKWrapper::EnvCreateWithJsonFile(const stl::path &file,
                                          std::string *response) const {
  return EnvCreateWithJson(ReadJsonFile(file), response);
}

int32_t SDKWrapper::EnvUpdateWithJson(const std::string &json,
                                      std::string *response) const {
  return CallSyncJson(fn_sdk_env_update_, json, response);
}

int32_t SDKWrapper::EnvUpdateWithJsonFile(const stl::path &file,
                                          std::string *response) const {
  return EnvUpdateWithJson(ReadJsonFile(file), response);
}

int32_t SDKWrapper::EnvPageWithJson(const std::string &json,
                                    std::string *response) const {
  return CallSyncJson(fn_sdk_env_page_, json, response);
}

int32_t SDKWrapper::EnvPageWithJsonFile(const stl::path &file,
                                        std::string *response) const {
  return EnvPageWithJson(ReadJsonFile(file), response);
}

int32_t SDKWrapper::EnvDestroyWithJson(const std::string &json,
                                       std::string *response) const {
  return CallSyncJson(fn_sdk_env_destroy_, json, response);
}

int32_t SDKWrapper::EnvDestroyWithJsonFile(const stl::path &file,
                                           std::string *response) const {
  return EnvDestroyWithJson(ReadJsonFile(file), response);
}

int32_t SDKWrapper::TokenUpdate(const std::string &user_sig) const {
  rapidjson::Document document(rapidjson::kObjectType);
  auto &allocator = document.GetAllocator();
  document.AddMember(
      "userSig", rapidjson::Value(user_sig.c_str(), allocator).Move(),
      allocator);
  return TokenUpdateWithJson(Utils::JsonToString(document));
}

int32_t SDKWrapper::TokenUpdateWithJson(const std::string &json) const {
  return CallAsyncJson(fn_sdk_token_update_, json);
}

int32_t SDKWrapper::TokenUpdateWithJsonFile(const stl::path &file) const {
  return TokenUpdateWithJson(ReadJsonFile(file));
}

int32_t SDKWrapper::Shutdown() const {
  std::lock_guard<std::mutex> lock(api_mutex_);
  if (!ready_.load() || !fn_sdk_shutdown_)
    return -1;
  return fn_sdk_shutdown_();
}

std::vector<DemoAsyncEvent> SDKWrapper::Events() const {
  std::lock_guard<std::mutex> lock(events_mutex_);
  return events_;
}

void SDKWrapper::ClearEvents() {
  std::lock_guard<std::mutex> lock(events_mutex_);
  events_.clear();
}

bool SDKWrapper::WaitForBrowserOpenResult(const std::string &env_id,
                                          int timeout_seconds,
                                          DemoAsyncEvent *event) const {
  return WaitForAnyEvent({"browser-open-success", "browser-open-failed"}, env_id,
                         timeout_seconds, event);
}

bool SDKWrapper::WaitForBrowserCloseResult(const std::string &env_id,
                                           int timeout_seconds,
                                           DemoAsyncEvent *event) const {
  return WaitForAnyEvent({"browser-close-success", "browser-close-failed"},
                         env_id, timeout_seconds, event);
}

bool SDKWrapper::WaitForAnyEvent(const std::set<std::string> &types,
                                 const std::string &env_id, int timeout_seconds,
                                 DemoAsyncEvent *event) const {
  const auto matcher = [&](const DemoAsyncEvent &item) {
    return types.find(item.type) != types.end() &&
           (env_id.empty() || item.env_id == env_id);
  };

  std::unique_lock<std::mutex> lock(events_mutex_);
  const auto find_match = [&]() -> std::optional<DemoAsyncEvent> {
    for (auto it = events_.rbegin(); it != events_.rend(); ++it) {
      if (matcher(*it))
        return *it;
    }
    return std::nullopt;
  };

  if (const auto matched = find_match()) {
    if (event)
      *event = *matched;
    return true;
  }

  const auto deadline =
      std::chrono::steady_clock::now() + std::chrono::seconds(timeout_seconds);
  while (events_cv_.wait_until(lock, deadline) != std::cv_status::timeout) {
    if (const auto matched = find_match()) {
      if (event)
        *event = *matched;
      return true;
    }
  }

  if (const auto matched = find_match()) {
    if (event)
      *event = *matched;
    return true;
  }

  return false;
}

void SDK_CALL SDKWrapper::SDKResultCallback(int32_t code, void *user_data,
                                            const char *data, size_t len) {
  if (!user_data || !data || len == 0)
    return;
  SDKWrapper *self = reinterpret_cast<SDKWrapper *>(user_data);
  self->OnAsyncEvent(code, std::string(data, len));
}

void SDKWrapper::OnAsyncEvent(int32_t code, const std::string &payload) {
  DemoAsyncEvent event;
  event.callback_code = code;
  event.received_at = Utils::Time();
  event.payload = payload;

  rapidjson::Document document;
  if (!document.Parse(payload.data(), payload.size()).HasParseError() &&
      document.IsObject()) {
    if (document.HasMember("type"))
      event.type = JsonScalarToString(document["type"]);
    if (document.HasMember("reqId"))
      event.req_id = JsonScalarToString(document["reqId"]);
    if (document.HasMember("data") && document["data"].IsObject()) {
      const auto &data = document["data"];
      if (data.HasMember("envId"))
        event.env_id = JsonScalarToString(data["envId"]);
    }
  }

  {
    std::lock_guard<std::mutex> lock(events_mutex_);
    events_.push_back(event);
    std::cout << event.received_at << "[ASYNC] code=" << DescribeCode(code);
    if (!event.type.empty())
      std::cout << " type=" << event.type;
    if (!event.req_id.empty())
      std::cout << " reqId=" << event.req_id;
    if (!event.env_id.empty())
      std::cout << " envId=" << event.env_id;
    std::cout << std::endl;
    std::cout << Utils::PrettyJson(payload) << std::endl;
  }
  events_cv_.notify_all();
}
