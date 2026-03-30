#include "main.h"
namespace {
const std::string testJwtToken =
    R"(eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJyaWQiOiIyMDM4NTMxNzg4MTA3NjgxNzkyIiwiYWlkIjoyMDM3NzIyMjU3NzE0ODQzNjQ4LCJ1aWQiOjIwMzc0NjkzMzAxNzM1OTU2NDgsImNpZCI6ImJmOGViMjk2ZjFjNWJhNzM3MzZhYTdlYmRjNDg3ZGJkIiwiaXNzIjoiYnMiLCJleHAiOjE3NzQ5NDUyNjd9.b7pTjRv5gr_SdjiZgM6w9ajZHsevXbdr1bxMJqJatto)";
const std::string testJwtTokenUpdate =
    R"(eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJyaWQiOiIyMDM4NTMxNzg4MTA3NjgxNzkyIiwiYWlkIjoyMDM3NzIyMjU3NzE0ODQzNjQ4LCJ1aWQiOjIwMzc0NjkzMzAxNzM1OTU2NDgsImNpZCI6ImJmOGViMjk2ZjFjNWJhNzM3MzZhYTdlYmRjNDg3ZGJkIiwiaXNzIjoiYnMiLCJleHAiOjE3NzQ5NDUyNjd9.b7pTjRv5gr_SdjiZgM6w9ajZHsevXbdr1bxMJqJatto)";

constexpr const char kCreateBrowserEnvTemplate[] = R"json(
{
  "envName": "",
  "remark": "",
  "serial": "",
  "customerId": "%s",
  "region": "",
  "proxy": "%s",
  "bridgeProxy": "%s",
  "ipChannel": "",
  "finger": {
    "system": "Windows 10",
    "kernel": "Chrome",
    "kernelVersion": "%s",
    "uaVersion": "%s",
    "ua": "",
    "language": [],
    "zone": "",
    "geographic": {
      "enable": 1,
      "useip": 1,
      "longitude": "",
      "latitude": "",
      "accuracy": ""
    },
    "dpi": "default",
    "fontFinger": 1,
    "font": {
      "enable": 1,
      "list": []
    },
    "webRTC": 5,
    "webRTCIP": "",
    "canvas": 4,
    "webGl": 1,
    "webGlInfo": 2,
    "webGlVendor": "",
    "webGlRenderer": "",
    "audioContext": 1,
    "speechVoices": 2,
    "mediaDevice": 2,
    "cpu": 4,
    "mem": 8,
    "deviceName": "",
    "mac": "",
    "hardware": 1,
    "bluetooth": 2,
    "doNotTrack": 2,
    "enableScanPort": 1,
    "scanPort": "",
    "enableOpen": 1,
    "enableNotice": 1,
    "enablePic": 2,
    "picSize": "",
    "enableGc": 2,
    "gcTime": 1,
    "battery": 1,
    "enableSound": 2,
    "enableVideo": 2,
	"searchEngine":1,
	"translateLang":1,
	"enableStorage":1,
	"enableDevtools":2,
	"clearCookie":2,
	"clearStorage":2,
	"shortName":2
  }
}
)json";

typedef bool(SDK_CALL *sdk_is_ok_t)(int32_t);
typedef bool(SDK_CALL *sdk_is_done_t)(int32_t);
typedef bool(SDK_CALL *sdk_is_error_t)(int32_t);
typedef bool(SDK_CALL *sdk_is_reqid_t)(int32_t);
typedef void(SDK_CALL *sdk_free_t)(void *);
typedef void *(SDK_CALL *sdk_malloc_t)(size_t);
typedef const char *(SDK_CALL *sdk_error_name_t)(int32_t);
typedef const char *(SDK_CALL *sdk_error_string_t)(int32_t);
typedef int32_t(SDK_CALL *sdk_init_t)(sdk_handle_t *, const char *, size_t,
                                      char **, size_t *);
typedef int32_t(SDK_CALL *sdk_token_update_t)(const char *, size_t);
typedef int32_t(SDK_CALL *sdk_shutdown_t)(void);
typedef int32_t(SDK_CALL *sdk_register_result_cb_t)(sdk_result_cb_t, void *);
typedef int32_t(SDK_CALL *sdk_register_result_cb_t)(sdk_result_cb_t, void *);
typedef int32_t(SDK_CALL *sdk_browser_install_t)(const char *data, size_t len);
typedef int32_t(SDK_CALL *sdk_browser_open_t)(const char *data, size_t len);
typedef int32_t(SDK_CALL *sdk_browser_close_t)(const char *data, size_t len);
typedef int32_t(SDK_CALL *sdk_env_create_t)(const char *data, size_t len,
                                            char **out_data, size_t *out_len);
typedef int32_t(SDK_CALL *sdk_env_update_t)(const char *data, size_t len,
                                            char **out_data, size_t *out_len);
typedef int32_t(SDK_CALL *sdk_env_page_t)(const char *data, size_t len,
                                          char **out_data, size_t *out_len);
typedef int32_t(SDK_CALL *sdk_env_destroy_t)(const char *data, size_t len,
                                             char **out_data, size_t *out_len);
typedef int32_t(SDK_CALL *sdk_info_t)(char **out_data, size_t *out_len);
typedef int32_t(SDK_CALL *sdk_browser_info_t)(char **out_data, size_t *out_len);
typedef int32_t(SDK_CALL *sdk_register_cookies_storage_cb_t)(
    sdk_cookies_storage_cb_t cb, void *user_data);

static sdk_error_name_t fn_sdk_error_name = nullptr;
static sdk_error_string_t fn_sdk_error_string = nullptr;
static sdk_free_t fn_sdk_free = nullptr;
static sdk_malloc_t fn_sdk_malloc = nullptr;
static sdk_is_ok_t fn_sdk_is_ok = nullptr;
static sdk_is_done_t fn_sdk_is_done = nullptr;
static sdk_is_error_t fn_sdk_is_error = nullptr;
static sdk_is_reqid_t fn_sdk_is_reqid = nullptr;
static sdk_browser_info_t fn_sdk_browser_info = nullptr;
static sdk_browser_install_t fn_sdk_browser_install = nullptr;
static sdk_browser_open_t fn_sdk_browser_open = nullptr;
static sdk_browser_open_t fn_sdk_browser_close = nullptr;
static sdk_env_create_t fn_sdk_env_create = nullptr;
static sdk_env_update_t fn_sdk_env_update = nullptr;
static sdk_env_destroy_t fn_sdk_env_destroy = nullptr;
static sdk_env_page_t fn_sdk_env_page = nullptr;
static sdk_init_t fn_sdk_init = nullptr;
static sdk_info_t fn_sdk_info = nullptr;
static sdk_shutdown_t fn_sdk_shutdown = nullptr;
static sdk_register_result_cb_t fn_sdk_register_result_cb = nullptr;
static sdk_token_update_t fn_sdk_token_update = nullptr;
static sdk_register_cookies_storage_cb_t fn_sdk_register_cookies_storage_cb =
    nullptr;
} // namespace
SDKWrapper::SDKWrapper() {
  InitInternal();
}
SDKWrapper::~SDKWrapper() {
  UnInitInternal();
}
void SDKWrapper::InitInternal() {
  current_dir_ = Utils::GetCurrentProcessDir();

  do {
    std::string cfg = Utils::ReadFile(current_dir_ / u"test.json");
    if (cfg.empty())
      break;
    rapidjson::Document doc;
    if (doc.Parse(cfg.data(), cfg.size()).HasParseError())
      break;
    if (!doc.IsObject())
      break;
    if (doc.HasMember("vers") && doc["vers"].IsArray()) {
      if (doc["vers"].Empty())
        break;
      YunLoginChromiumVersions.clear();
      for (auto it = doc["vers"].Begin(); it != doc["vers"].End(); ++it) {
        if (!it->IsString())
          break;
        if (it->GetStringLength() <= 0)
          continue;
        YunLoginChromiumVersions.emplace_back(it->GetString());
      }
    }
  } while (0);

  module_path_ = (current_dir_ / module_name_).lexically_normal();
  do {
    module_ = dlopen(module_path_.string().c_str(), RTLD_NOW);
    // Resolve required symbols
    fn_sdk_browser_install = reinterpret_cast<sdk_browser_install_t>(
        dlsym(module_, "sdk_browser_install"));
    fn_sdk_browser_open = reinterpret_cast<sdk_browser_open_t>(
        dlsym(module_, "sdk_browser_open"));
    fn_sdk_browser_close = reinterpret_cast<sdk_browser_close_t>(
        dlsym(module_, "sdk_browser_close"));
    fn_sdk_browser_info = reinterpret_cast<sdk_browser_info_t>(
        dlsym(module_, "sdk_browser_info"));
    fn_sdk_env_create =
        reinterpret_cast<sdk_env_create_t>(dlsym(module_, "sdk_env_create"));
    fn_sdk_env_page =
        reinterpret_cast<sdk_env_page_t>(dlsym(module_, "sdk_env_page"));
    fn_sdk_env_update =
        reinterpret_cast<sdk_env_update_t>(dlsym(module_, "sdk_env_update"));
    fn_sdk_env_destroy =
        reinterpret_cast<sdk_env_destroy_t>(dlsym(module_, "sdk_env_destroy"));
    fn_sdk_error_name =
        reinterpret_cast<sdk_error_name_t>(dlsym(module_, "sdk_error_name"));
    fn_sdk_error_string = reinterpret_cast<sdk_error_string_t>(
        dlsym(module_, "sdk_error_string"));
    fn_sdk_free = reinterpret_cast<sdk_free_t>(dlsym(module_, "sdk_free"));
    fn_sdk_is_ok = reinterpret_cast<sdk_is_ok_t>(dlsym(module_, "sdk_is_ok"));
    fn_sdk_is_done =
        reinterpret_cast<sdk_is_done_t>(dlsym(module_, "sdk_is_done"));
    fn_sdk_is_error =
        reinterpret_cast<sdk_is_error_t>(dlsym(module_, "sdk_is_error"));
    fn_sdk_is_reqid =
        reinterpret_cast<sdk_is_reqid_t>(dlsym(module_, "sdk_is_reqid"));
    fn_sdk_init = reinterpret_cast<sdk_init_t>(dlsym(module_, "sdk_init"));
    fn_sdk_info = reinterpret_cast<sdk_info_t>(dlsym(module_, "sdk_info"));
    fn_sdk_shutdown =
        reinterpret_cast<sdk_shutdown_t>(dlsym(module_, "sdk_shutdown"));
    fn_sdk_register_result_cb = reinterpret_cast<sdk_register_result_cb_t>(
        dlsym(module_, "sdk_register_result_cb"));
    fn_sdk_token_update = reinterpret_cast<sdk_token_update_t>(
        dlsym(module_, "sdk_token_update"));
    fn_sdk_register_cookies_storage_cb =
        reinterpret_cast<sdk_register_cookies_storage_cb_t>(
            dlsym(module_, "sdk_register_cookies_storage_cb"));
    fn_sdk_malloc =
        reinterpret_cast<sdk_malloc_t>(dlsym(module_, "sdk_malloc"));
    if (!fn_sdk_error_name || !fn_sdk_error_string || !fn_sdk_shutdown ||
        !fn_sdk_malloc || !fn_sdk_register_cookies_storage_cb ||
        !fn_sdk_register_result_cb || !fn_sdk_is_done || !fn_sdk_is_ok ||
        !fn_sdk_is_reqid || !fn_sdk_is_error || !fn_sdk_token_update ||
        !fn_sdk_env_create || !fn_sdk_env_update || !fn_sdk_env_page ||
        !fn_sdk_env_destroy || !fn_sdk_info || !fn_sdk_browser_info ||
        !fn_sdk_browser_install)
      break;
    fn_sdk_register_result_cb(SDKWrapper::SDKResultCallback, this);
    fn_sdk_register_cookies_storage_cb(SDKWrapper::SDKCookiesStorageCallback,
                                       this);
    ready_.store(true);

  } while (0);
  threads_.emplace_back([this]() { AutoProcess(); });
}
void SDKWrapper::UnInitInternal() {
  ready_.store(false);
  for (auto &t : threads_)
    t.join();
  threads_.clear();
  if (module_) {
#if defined(IS_WINDOWS)
    FreeLibrary(reinterpret_cast<HMODULE>(module_));
    module_ = nullptr;
#elif defined(IS_LINUX) || defined(IS_APPLE)
    dlclose(module_);
    module_ = nullptr;
#endif
  }
  ready_.store(false);
}
bool SDKWrapper::Ready() const {
  std::lock_guard<std::mutex> lk(mtx_);
  return ready_.load();
}
bool SDKWrapper::BrowserEnvPage(const std::string &body) {
  bool result = false;
  std::lock_guard<std::mutex> lk(mtx_);
  do {
    if (!fn_sdk_env_page)
      break;
    char *out = nullptr;
    size_t len = 0;
    fn_sdk_env_page(body.data(), body.size(),&out,&len);
    std::cout << std::string(out, len) << std::endl;
   result = true;
  } while (0);
  return result;
}
bool SDKWrapper::BrowserEnvUpdate(const std::string &body) {
  bool result = false;
  std::lock_guard<std::mutex> lk(mtx_);
  do {
    if (!fn_sdk_env_update)
      break;
    char *out = nullptr;
    size_t len = 0;
    fn_sdk_env_update(body.data(), body.size(), &out, &len);
    std::cout << std::string(out, len) << std::endl;
    result = true;
  } while (0);
  return result;
}

void SDK_CALL SDKWrapper::SDKCookiesStorageCallback(const char *data,
                                                    size_t len, char **new_data,
                                                    size_t *new_len,
                                                    void *user_data) {
  do {
    if (!data || len <= 0)
      break;
    std::cout << std::string(data, len) << std::endl;
    // const std::string newCookies = "[]";
    //*new_len = newCookies.size();
    //*new_data = (char*)fn_sdk_malloc(*new_len);
    // memcpy(*new_data, newCookies.data(), *new_len);
  } while (0);
}
void SDK_CALL SDKWrapper::SDKResultCallback(int32_t code, void *user_data,
                                            const char *data, size_t len) {

  if (data && len > 0) {
    std::string strData(data, len);
    std::cout << Utils::Time() << "[DEMO][NOTIFY][ASYNC]: " << strData
              << std::endl;

    // Parse progress body and extract envId + type so AutoProcess can wait
    // for per-env opened/closed events.
    SDKWrapper *self = reinterpret_cast<SDKWrapper *>(user_data);
    if (!self)
      return;
    rapidjson::Document doc;
    if (doc.Parse(strData.c_str()).HasParseError())
      return;
    std::string type;
    if (doc.HasMember("type") && doc["type"].IsString())
      type = doc["type"].GetString();
    if (!doc.HasMember("data") || !doc["data"].IsObject())
      return;
    const auto &d = doc["data"];
    std::string udd;
    if (!d.HasMember("envId") || !d["envId"].IsString())
      return;
    if (d.HasMember("udd") && d["udd"].IsString())
      udd = d["udd"].GetString();
    uint64_t envId = 0;
    try {
      envId = std::stoull(d["envId"].GetString());
    } catch (...) {
      return;
    }

    {
      std::lock_guard<std::mutex> lk(self->notify_mtx_);
      // Match exact event type strings produced by GetLogEventIDTypeStr().
      if (type == "browser-open-success") {
        self->opened_envs_.insert(envId);
      } else if (type == "browser-close-success") {
        self->closed_envs_.insert(envId);
      }
      // Track all completed open/close attempts (success or failure) so
      // AutoProcess can detect batch completion even when some browsers fail.
      if (type == "browser-open-success" || type == "browser-open-failed") {
        self->open_done_envs_.insert(envId);
      } else if (type == "browser-close-success" ||
                 type == "browser-close-failed") {
        // if (!udd.empty()) {
        //   path delTarget =
        //       (Utils::GetCurrentProcessDir() / u".brosdk/userData/" / udd)
        //           .lexically_normal();
        //   if (!Utils::RemoveDir(delTarget)) {
        //     std::lock_guard<std::mutex> lk(self->mtx_deldir_);
        //     self->deldirs_.insert(delTarget);
        //   }
        // }
        self->close_done_envs_.insert(envId);
      }
    }
    self->notify_cv_.notify_all();
  }
}

bool SDKWrapper::WaitForEnvEvents(const std::set<uint64_t> &envIds,
                                  const std::set<uint64_t> &targetSet,
                                  int timeoutSeconds) {
  std::unique_lock<std::mutex> lk(notify_mtx_);
  auto deadline =
      std::chrono::steady_clock::now() + std::chrono::seconds(timeoutSeconds);
  return notify_cv_.wait_until(lk, deadline, [&]() {
    for (auto id : envIds) {
      if (targetSet.find(id) == targetSet.end())
        return false;
    }
    return true;
  });
}
bool SDKWrapper::GetInfo(std::string &out) const {
  out.clear();
  std::lock_guard<std::mutex> lk(mtx_);
  char *info = nullptr;
  size_t len = 0;
  if (0 != fn_sdk_info(&info, &len))
    return false;
  out.assign(info, len);
  fn_sdk_free(info);
  return true;
}
bool SDKWrapper::Init() {
  bool result = false;
  std::lock_guard<std::mutex> lk(mtx_);
  do {
    if (!ready_.load())
      break;
    std::string workDir = (Utils::GetAppDataDir() / u"BroSDK-Dev")
                              .lexically_normal()
                              .generic_string();
    // std::string sdkApiUrl = "http://192.168.0.188:9988";
#if defined(IS_APPLE)
    char buf[0xffff] = {0};
    int len = std::snprintf(buf, sizeof(buf),
                            R"("port":%d, "userSig": "%s","workDir":"%s")",
                            65535, testJwtToken.c_str(), workDir.c_str());
    std::string reqBody(buf, len);
    reqBody.insert(0, "{");
    reqBody.push_back('}');
#elif defined(IS_WINDOWS)
    std::string reqBody =
        //"sdkApiUrl":"{}"
        std::format(R"({{"port":{}, "userSig": "{}","workDir":"{}"}})", 65535,
                    testJwtToken, workDir);
#endif
    char *out_data = nullptr;
    size_t out_len = 0;
    int32_t err = fn_sdk_init(nullptr, reqBody.c_str(), reqBody.size(),
                              &out_data, &out_len);
    result = !fn_sdk_is_error(err);
    if (!result) {
      reqBody = std::string("{\"userSig\": \"") + testJwtTokenUpdate + "\"}";
      fn_sdk_token_update(reqBody.data(), reqBody.size());
      std::cerr << "[DEMO][NOTIFY][SYNC]: Failed to init web API: "
                << fn_sdk_error_string(err) << std::endl;
    }
    if (out_data && out_len > 0) {
      std::cout << Utils::Time()
                << "[DEMO][NOTIFY][SYNC]: " << std::string(out_data, out_len)
                << std::endl;
    }
    fn_sdk_free(out_data);
  } while (0);
  return result;
}
uint64_t SDKWrapper::BrowserEnvCreate(const std::string &ver,
                                      const std::string &proxy,
                                      const std::string &bridgeProxy) {
  uint64_t result = 0;
  std::lock_guard<std::mutex> lk(mtx_);
  do {
    if (!ready_.load())
      break;
    char buf[0xffff] = {0};
#if defined(IS_WINDOWS)
    int len =
        sprintf_s(buf, sizeof(buf), kCreateBrowserEnvTemplate,
                  std::to_string(Utils::TimeStamp()).c_str(), proxy.c_str(),
                  bridgeProxy.c_str(), ver.c_str(), ver.c_str());
#else
    int len =
        std::snprintf(buf, sizeof(buf), kCreateBrowserEnvTemplate,
                      std::to_string(Utils::TimeStamp()).c_str(), proxy.c_str(),
                      bridgeProxy.c_str(), ver.c_str(), ver.c_str());
#endif
    if (len >= static_cast<int>(sizeof(buf)))
      break;
    buf[len] = 0;
    rapidjson::Document doc;
    if (doc.Parse(buf).HasParseError())
      break;
    std::string reqBody = Utils::JsonToString(doc);
    char *out_data = nullptr;
    size_t out_len = 0;
    int32_t err =
        fn_sdk_env_create(reqBody.data(), reqBody.size(), &out_data, &out_len);
    if (err != 1 && err != 0) {
      std::cerr << "BrowserOpen returned: " << fn_sdk_error_string(err)
                << std::string(out_data,out_len) 
                << std::endl;
      break;
    }
    if (!out_data || out_len == 0)
      break;
    rapidjson::Document resDoc;
    if (resDoc.Parse(out_data, out_len).HasParseError())
      break;
    if (!resDoc.HasMember("data") || !resDoc["data"].IsObject())
      break;
    if (resDoc["data"].HasMember("envId") && resDoc["data"]["envId"].IsString())
      result = strtoull(resDoc["data"]["envId"].GetString(), nullptr, 10);
  } while (0);
  return result;
}
bool SDKWrapper::BrowserInstall(const std::string &body) {
  bool result = false;
  std::lock_guard<std::mutex> lk(mtx_);
  do {
    if (body.empty())
      break;
    int32_t err = fn_sdk_browser_install(body.data(), body.size());
    if (err != 1 && err != 0) {
      std::cerr << "BrowserInstall returned: " << fn_sdk_error_string(err)
                << std::endl;
    }
    result = err == 1 || err == 0;
    result = true;
  } while (0);
  return result;
}
bool SDKWrapper::BrowserOpen(const char *data, size_t len) {
  bool result = false;
  std::lock_guard<std::mutex> lk(mtx_);
  do {
    if (!ready_.load())
      break;
    if (!data || len == 0)
      break;
    int32_t err = fn_sdk_browser_open(data, len);
    if (err != 1 && err != 0) {
      std::cerr << "BrowserOpen returned: " << fn_sdk_error_string(err)
                << std::endl;
    }
    result = err == 1 || err == 0;
  } while (0);
  return result;
}
bool SDKWrapper::BrowserClose(const char *data, size_t len) {
  bool result = false;
  std::lock_guard<std::mutex> lk(mtx_);
  do {
    if (!ready_.load())
      break;
    int32_t err = fn_sdk_browser_close(data, len);
    result = err == 1;
  } while (0);
  return result;
}
bool SDKWrapper::Shutdown() {
  bool result = false;
  std::lock_guard<std::mutex> lk(mtx_);
  result = fn_sdk_is_ok(fn_sdk_shutdown());
  return result;
}
bool SDKWrapper::GenOpenBrwReqBody(std::set<uint64_t> &envIds,
                                   const std::vector<std::string> &appendArgs,
                                   const std::string &urlsString,
                                   const std::string &cookiesString,
                                   std::string &reqBody) {
  bool result = false;
  std::lock_guard<std::mutex> lk(mtx_);
  do {
    if (envIds.empty())
      break;
    rapidjson::Document doc(rapidjson::Type::kObjectType);
    rapidjson::Value envs(rapidjson::Type::kArrayType);
    for (const auto &envId : envIds) {
      rapidjson::Value env(rapidjson::Type::kObjectType);
      env.AddMember(
          rapidjson::Value().SetString("envId", doc.GetAllocator()).Move(),
          rapidjson::Value()
              .SetString(std::to_string(envId).c_str(), doc.GetAllocator())
              .Move(),
          doc.GetAllocator());

      rapidjson::Value args(rapidjson::Type::kArrayType);
      rapidjson::Value urls(rapidjson::Type::kArrayType);
      rapidjson::Value cookies(rapidjson::Type::kArrayType);
      rapidjson::Value extensions(rapidjson::Type::kArrayType);
      args.PushBack(rapidjson::Value()
                        .SetString("--no-first-run", doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());
      args.PushBack(
          rapidjson::Value()
              .SetString("--no-default-browser-check", doc.GetAllocator())
              .Move(),
          doc.GetAllocator());
      for (const auto &arg : appendArgs) {
        args.PushBack(rapidjson::Value()
                          .SetString(arg.c_str(), doc.GetAllocator())
                          .Move(),
                      doc.GetAllocator());
      }
      env.AddMember(
          rapidjson::Value().SetString("args", doc.GetAllocator()).Move(),
          args.Move(), doc.GetAllocator());
      bool cookiesParsed = false;
      bool urlsParsed = false;
      do {
        if (urlsString.empty())
          break;
        rapidjson::Document tmp;
        if (tmp.Parse(urlsString.data(), urlsString.size()).HasParseError())
          break;
        if (!tmp.IsArray())
          break;
        urls.CopyFrom(tmp, doc.GetAllocator());
        urlsParsed = true;
      } while (0);
      do {
        if (cookiesString.empty())
          break;
        rapidjson::Document tmp;
        if (tmp.Parse(cookiesString.data(), cookiesString.size())
                .HasParseError())
          break;
        if (!tmp.IsArray())
          break;
        cookies.CopyFrom(tmp, doc.GetAllocator());
        cookiesParsed = true;
      } while (0);

      if (urlsParsed) {
        env.AddMember(
            rapidjson::Value().SetString("urls", doc.GetAllocator()).Move(),
            urls.Move(), doc.GetAllocator());
      }

      if (cookiesParsed) {
        env.AddMember(
            rapidjson::Value().SetString("cookies", doc.GetAllocator()).Move(),
            cookies.Move(), doc.GetAllocator());
      }

      do {
        const std::string exts = R"json(
[
	{
		"name": "testExt1",
		"id": "ebglcogbaklfalmoeccdjbmgfcacengf",
		"packType": "unpack",
		"component": false,
		"data": {
			"key1": "aGVsbG8=",
			"key3": "5L2g5aW9",
			"key2": "12345234634574568478asdfdgsdfg"
		}
	},
	{
		"name": "testExt2",
		"id": "afgbmmdnakcefnkchckgelobigkbboci",
		"packType": "unpack",
		"component": false,
		"data": {
			"data1": "5Zyo5ZCX77yfCg==",
			"ddataTest22345": "d2VsY29tZQo=",
			"testData": "!@#%^@$#^#$%&#%^*$%^*$^&*%^&*"
		}
	}
]
)json";
        rapidjson::Document docExts;
        if (docExts.Parse(exts.data(), exts.size()).HasParseError())
          break;
        if (!docExts.IsArray())
          break;
        extensions.CopyFrom(docExts.Move(), doc.GetAllocator());
        env.AddMember(
            rapidjson::Value().SetString("extensions", doc.GetAllocator()).Move(),
                      extensions.Move(), doc.GetAllocator());
      } while (0);

      envs.PushBack(env.Move(), doc.GetAllocator());
    }
    
    





    doc.AddMember(
        rapidjson::Value().SetString("envs", doc.GetAllocator()).Move(),
        envs.Move(), doc.GetAllocator());
    reqBody = Utils::JsonToString(doc);

    result = true;
  } while (0);
  return result;
}
bool SDKWrapper::GenCloseBrwReqBody(const std::set<uint64_t> &envIds,
                                    std::string &reqBody) {
  bool result = false;
  std::lock_guard<std::mutex> lk(mtx_);
  return GenCloseBrwReq(envIds, reqBody);
}
bool SDKWrapper::GenCloseBrwReq(const std::set<uint64_t> &envIds,
                                std::string &reqBody) {
  bool result = false;
  do {
    if (envIds.empty())
      break;
    rapidjson::Document doc(rapidjson::Type::kObjectType);
    rapidjson::Value envs(rapidjson::Type::kArrayType);
    for (const auto &envId : envIds) {
      envs.PushBack(
          rapidjson::Value()
              .SetString(std::to_string(envId).c_str(), doc.GetAllocator())
              .Move(),
          doc.GetAllocator());
    }
    doc.AddMember(
        rapidjson::Value().SetString("envs", doc.GetAllocator()).Move(),
        envs.Move(), doc.GetAllocator());
    reqBody = Utils::JsonToString(doc);

    result = true;
  } while (0);
  return result;
}

bool SDKWrapper::GenOpenBrwReq(const size_t &count, std::set<uint64_t> &envIds,
                               const std::vector<std::string> &appendArgs,
                               const std::string &urlsString,
                               const std::string &cookiesString,
                               std::string &reqBody) {
  bool result = false;
  envIds.clear();
  reqBody.clear();
  do {
    if (count <= 0)
      break;
    for (size_t i = 0; i < count; ++i) {
      auto r = Utils::CreateBrowserEnv(
          std::string("id-").append(std::to_string(i)),
          YunLoginChromiumVersions[Utils::GetRandomValue<size_t>(
              0, YunLoginChromiumVersions.size() - 1)],
          "socks5://faze06a2:faze06a2@49.7.133.120:2018");
      if (r > 0) {
        envIds.insert(r);
      }
    }
    if (envIds.empty())
      break;
    rapidjson::Document doc(rapidjson::Type::kObjectType);
    rapidjson::Value envs(rapidjson::Type::kArrayType);
    for (const auto &envId : envIds) {
      rapidjson::Value env(rapidjson::Type::kObjectType);
      env.AddMember(
          rapidjson::Value().SetString("envId", doc.GetAllocator()).Move(),
          rapidjson::Value()
              .SetString(std::to_string(envId).c_str(), doc.GetAllocator())
              .Move(),
          doc.GetAllocator());

      rapidjson::Value args(rapidjson::Type::kArrayType);
      rapidjson::Value urls(rapidjson::Type::kArrayType);
      rapidjson::Value cookies(rapidjson::Type::kArrayType);

      args.PushBack(rapidjson::Value()
                        .SetString("--no-first-run", doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());
      args.PushBack(
          rapidjson::Value()
              .SetString("--no-default-browser-check", doc.GetAllocator())
              .Move(),
          doc.GetAllocator());
      for (const auto &arg : appendArgs) {
        args.PushBack(rapidjson::Value()
                          .SetString(arg.c_str(), doc.GetAllocator())
                          .Move(),
                      doc.GetAllocator());
      }
      env.AddMember(
          rapidjson::Value().SetString("args", doc.GetAllocator()).Move(),
          args.Move(), doc.GetAllocator());

      do {
        rapidjson::Document doc;
        if (doc.Parse(urlsString.data(), urlsString.size()).HasParseError())
          break;
        if (!doc.IsArray())
          break;
        urls.CopyFrom(doc, doc.GetAllocator());
      } while (0);
      do {
        rapidjson::Document doc;
        if (doc.Parse(cookiesString.data(), cookiesString.size())
                .HasParseError())
          break;
        if (!doc.IsArray())
          break;
        cookies.CopyFrom(doc, doc.GetAllocator());
      } while (0);
      env.AddMember(
          rapidjson::Value().SetString("urls", doc.GetAllocator()).Move(),
          urls.Move(), doc.GetAllocator());
      env.AddMember(
          rapidjson::Value().SetString("cookies", doc.GetAllocator()).Move(),
          cookies.Move(), doc.GetAllocator());
      envs.PushBack(env.Move(), doc.GetAllocator());
    }
    doc.AddMember(
        rapidjson::Value().SetString("envs", doc.GetAllocator()).Move(),
        envs.Move(), doc.GetAllocator());
    reqBody = Utils::JsonToString(doc);
    result = true;
  } while (0);
  return result;
}

// const std::vector<std::string> kYunLoginChromiumVersions = {
//     "119", "122", "127", "131", "134", "138", "140",
// };

// const std::vector<std::string> kYunLoginChromiumVersions = {"127", "131",
// "134",
//                                                             "138"};