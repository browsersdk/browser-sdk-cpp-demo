#if !defined(__SDK_WRAPPER_H__)
#define __SDK_WRAPPER_H__

using sdk_register_result_cb_t = int32_t(SDK_CALL *)(sdk_result_cb_t, void *);
using sdk_init_t = int32_t(SDK_CALL *)(sdk_handle_t *, const char *, size_t,
                                       char **, size_t *);
using sdk_info_t = int32_t(SDK_CALL *)(char **, size_t *);
using sdk_browser_install_t = int32_t(SDK_CALL *)(const char *, size_t);
using sdk_browser_info_t = int32_t(SDK_CALL *)(char **, size_t *);
using sdk_browser_open_t = int32_t(SDK_CALL *)(const char *, size_t);
using sdk_browser_close_t = int32_t(SDK_CALL *)(const char *, size_t);
using sdk_env_create_t = int32_t(SDK_CALL *)(const char *, size_t, char **,
                                             size_t *);
using sdk_env_update_t = int32_t(SDK_CALL *)(const char *, size_t, char **,
                                             size_t *);
using sdk_env_page_t = int32_t(SDK_CALL *)(const char *, size_t, char **,
                                           size_t *);
using sdk_env_destroy_t = int32_t(SDK_CALL *)(const char *, size_t, char **,
                                              size_t *);
using sdk_token_update_t = int32_t(SDK_CALL *)(const char *, size_t);
using sdk_shutdown_t = int32_t(SDK_CALL *)(void);
using sdk_free_t = void(SDK_CALL *)(void *);
using sdk_error_name_t = const char *(SDK_CALL *)(int32_t);
using sdk_error_string_t = const char *(SDK_CALL *)(int32_t);

struct DemoInitOptions {
  std::string user_sig;
  std::string work_dir;
  std::string sdk_api_url;
  uint16_t port = 0;
  bool debug = true;
};

struct DemoBrowserOpenOptions {
  std::string env_id;
  std::vector<std::string> urls;
  std::vector<std::string> args;
};

struct DemoAsyncEvent {
  int32_t callback_code = 0;
  std::string received_at;
  std::string type;
  std::string req_id;
  std::string env_id;
  std::string payload;
};

class SDKWrapper {
public:
  SDKWrapper();
  ~SDKWrapper();
  SDKWrapper(const SDKWrapper &) = delete;
  SDKWrapper &operator=(const SDKWrapper &) = delete;

public:
  bool Ready() const;
  std::string LastError() const;
  stl::path ModulePath() const;
  std::string DescribeCode(int32_t code) const;

public:
  int32_t Init(const DemoInitOptions &options, std::string *response);
  int32_t InitWithJson(const std::string &json, std::string *response);
  int32_t InitWithJsonFile(const stl::path &file, std::string *response);
  int32_t Info(std::string *response) const;
  int32_t BrowserInfo(std::string *response) const;
  int32_t BrowserInstallWithJson(const std::string &json) const;
  int32_t BrowserInstallWithJsonFile(const stl::path &file) const;
  int32_t BrowserOpen(const DemoBrowserOpenOptions &options) const;
  int32_t BrowserOpenWithJson(const std::string &json) const;
  int32_t BrowserOpenWithJsonFile(const stl::path &file) const;
  int32_t BrowserClose(const std::vector<std::string> &env_ids) const;
  int32_t BrowserCloseWithJson(const std::string &json) const;
  int32_t BrowserCloseWithJsonFile(const stl::path &file) const;
  int32_t EnvCreateWithJson(const std::string &json,
                            std::string *response) const;
  int32_t EnvCreateWithJsonFile(const stl::path &file,
                                std::string *response) const;
  int32_t EnvUpdateWithJson(const std::string &json,
                            std::string *response) const;
  int32_t EnvUpdateWithJsonFile(const stl::path &file,
                                std::string *response) const;
  int32_t EnvPageWithJson(const std::string &json,
                          std::string *response) const;
  int32_t EnvPageWithJsonFile(const stl::path &file,
                              std::string *response) const;
  int32_t EnvDestroyWithJson(const std::string &json,
                             std::string *response) const;
  int32_t EnvDestroyWithJsonFile(const stl::path &file,
                                 std::string *response) const;
  int32_t TokenUpdate(const std::string &user_sig) const;
  int32_t TokenUpdateWithJson(const std::string &json) const;
  int32_t TokenUpdateWithJsonFile(const stl::path &file) const;
  int32_t Shutdown() const;

public:
  std::vector<DemoAsyncEvent> Events() const;
  void ClearEvents();
  bool WaitForBrowserOpenResult(const std::string &env_id, int timeout_seconds,
                                DemoAsyncEvent *event = nullptr) const;
  bool WaitForBrowserCloseResult(const std::string &env_id,
                                 int timeout_seconds,
                                 DemoAsyncEvent *event = nullptr) const;

private:
  using sync_noargs_fn = int32_t(SDK_CALL *)(char **, size_t *);
  using sync_json_fn = int32_t(SDK_CALL *)(const char *, size_t, char **,
                                          size_t *);
  using async_json_fn = int32_t(SDK_CALL *)(const char *, size_t);

private:
  bool LoadModule();
  void UnloadModule();
  bool ResolveFunctions();
  int32_t CallSyncNoArgs(sync_noargs_fn fn, std::string *response) const;
  int32_t CallSyncJson(sync_json_fn fn, const std::string &json,
                       std::string *response) const;
  int32_t CallAsyncJson(async_json_fn fn, const std::string &json) const;
  bool WaitForAnyEvent(const std::set<std::string> &types,
                       const std::string &env_id, int timeout_seconds,
                       DemoAsyncEvent *event) const;
  std::string BuildInitJson(const DemoInitOptions &options) const;
  std::string BuildOpenJson(const DemoBrowserOpenOptions &options) const;
  std::string BuildCloseJson(const std::vector<std::string> &env_ids) const;
  static void SDK_CALL SDKResultCallback(int32_t code, void *user_data,
                                         const char *data, size_t len);
  void OnAsyncEvent(int32_t code, const std::string &payload);

private:
  void *module_ = nullptr;
  stl::path current_dir_;
  stl::path module_path_;
  std::string last_error_;
  std::atomic<bool> ready_ = false;
  mutable std::mutex api_mutex_;
  mutable std::mutex events_mutex_;
  mutable std::condition_variable events_cv_;
  std::vector<DemoAsyncEvent> events_;

private:
  sdk_register_result_cb_t fn_sdk_register_result_cb_ = nullptr;
  sdk_init_t fn_sdk_init_ = nullptr;
  sdk_info_t fn_sdk_info_ = nullptr;
  sdk_browser_install_t fn_sdk_browser_install_ = nullptr;
  sdk_browser_info_t fn_sdk_browser_info_ = nullptr;
  sdk_browser_open_t fn_sdk_browser_open_ = nullptr;
  sdk_browser_close_t fn_sdk_browser_close_ = nullptr;
  sdk_env_create_t fn_sdk_env_create_ = nullptr;
  sdk_env_update_t fn_sdk_env_update_ = nullptr;
  sdk_env_page_t fn_sdk_env_page_ = nullptr;
  sdk_env_destroy_t fn_sdk_env_destroy_ = nullptr;
  sdk_token_update_t fn_sdk_token_update_ = nullptr;
  sdk_shutdown_t fn_sdk_shutdown_ = nullptr;
  sdk_free_t fn_sdk_free_ = nullptr;
  sdk_error_name_t fn_sdk_error_name_ = nullptr;
  sdk_error_string_t fn_sdk_error_string_ = nullptr;
};

#endif //__SDK_WRAPPER_H__
