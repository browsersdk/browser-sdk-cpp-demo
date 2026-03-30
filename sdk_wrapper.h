#if !defined(__SDK_WRAPPER_H__)
#define __SDK_WRAPPER_H__

class SDKWrapper {
public:
  SDKWrapper();
  ~SDKWrapper();
  bool Ready() const;

public:
  bool GetInfo(std::string &) const;
  bool Init();
  bool Shutdown();
  uint64_t BrowserEnvCreate(const std::string &ver, const std::string &proxy,
                            const std::string &bridgeProxy);
  bool BrowserOpen(const char *data, size_t len);
  bool BrowserClose(const char *data, size_t len);
  bool BrowserInstall(const std::string&);
  bool BrowserEnvPage(const std::string &);
  bool BrowserEnvUpdate(const std::string &);
  void StartAutoTest();
  bool GenOpenBrwReqBody(std::set<uint64_t> &,
                         const std::vector<std::string> &appendArgs,
                         const std::string &urls, const std::string &cookies,
                         std::string &reqBody);
  bool GenCloseBrwReqBody(const std::set<uint64_t> &, std::string &reqBody);

private:
  bool GenOpenBrwReqAuto(const size_t &count, std::set<uint64_t> &,
                         std::string &reqBody);
  bool GenOpenBrwReqAuto2(const size_t &count, std::set<uint64_t> &,
                          std::string &reqBody);
  bool GenOpenBrwReq(const size_t &count, std::set<uint64_t> &,
                     const std::vector<std::string> &appendArgs,
                     const std::string &urls, const std::string &cookies,
                     std::string &reqBody);
  bool GenCloseBrwReq(const std::set<uint64_t> &, std::string &reqBody);
  void AutoProcess();
  void InitInternal();
  void UnInitInternal();
  static void SDK_CALL SDKResultCallback(int32_t reqid_or_code, void *user_data,
                                         const char *data, size_t len);
  static void SDK_CALL SDKCookiesStorageCallback(const char *data, size_t len,
                                                 char **new_data,
                                                 size_t *new_len,
                                                 void *user_data);

private:
  void *module_ = nullptr;
  const std::u16string module_name_ =
#if defined(IS_APPLE)
      u"brosdk.dylib";
#elif defined(IS_WINDOWS)
      u"brosdk.dll";
#elif defined(IS_LINUX)
      u"brosdk.so";
#else
      u"brosdk";
#endif
  path current_dir_;
  path module_path_;
  std::atomic<bool> ready_ = false;
  std::atomic<bool> auto_test_running_ = false;
  std::atomic<size_t> auto_test_count_ = 0;
  std::vector<std::thread> threads_;
  mutable std::mutex mtx_;
  // Notification tracking for async SDK callbacks
  mutable std::mutex notify_mtx_;
  std::condition_variable notify_cv_;
  std::set<uint64_t> opened_envs_; // envIds that opened successfully
  std::set<uint64_t>
      open_done_envs_; // envIds with any open result (success or fail)
  std::set<uint64_t> closed_envs_; // envIds that closed successfully
  std::set<uint64_t>
      close_done_envs_; // envIds with any close result (success or fail)
  bool WaitForEnvEvents(const std::set<uint64_t> &envIds,
                        const std::set<uint64_t> &targetSet,
                        int timeoutSeconds);

private:
  // std::vector<std::string> YunLoginChromiumVersions = {"127", "131", "134",
  //                                                      "140"};
  std::vector<std::string> YunLoginChromiumVersions = {
      "119",
      "122",
  };

  mutable std::mutex mtx_deldir_;
  std::set<path> deldirs_;
};

#endif //__SDK_WRAPPER_H__