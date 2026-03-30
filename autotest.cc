#include "main.h"
namespace {
static std::set<uint64_t> __envIds__;
}
void SDKWrapper::StartAutoTest() {
  std::lock_guard<std::mutex> lk(mtx_);
  if (!auto_test_running_.load())
    auto_test_running_.store(true);
}
bool SDKWrapper::GenOpenBrwReqAuto2(const size_t &count,
                                   std::set<uint64_t> &envIds,
                                   std::string &reqBody) {
  bool result = false;
  envIds.clear();
  reqBody.clear();
  do {
    if (count <= 0)
      break;
    if (__envIds__.empty()) {
      for (int i = 0; i < 20; ++i) {
        auto r = BrowserEnvCreate(
            YunLoginChromiumVersions[Utils::GetRandomValue<size_t>(
                0, YunLoginChromiumVersions.size() - 1)],"","");
        __envIds__.insert(r);
      }
    }

    std::vector<uint64_t> v(__envIds__.begin(), __envIds__.end());
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);
    std::vector<uint64_t> fff(v.begin(), v.begin() + std::min<size_t>(Utils::GetRandomValue<size_t>(1,19),
                                                v.size()));

    for (const auto &it : fff)
      envIds.insert(it);

    rapidjson::Document doc(rapidjson::Type::kObjectType);
    rapidjson::Value envs(rapidjson::Type::kArrayType);

    // Tile windows to exactly fill the primary display in logical pixels.
#if defined(IS_WINDOWS)
    const int screen_width_px = GetSystemMetrics(SM_CXSCREEN);
    const int screen_height_px = GetSystemMetrics(SM_CYSCREEN);
#elif defined(IS_APPLE)
    const int screen_width_px =
        static_cast<int>(CGDisplayPixelsWide(CGMainDisplayID()));
    const int screen_height_px =
        static_cast<int>(CGDisplayPixelsHigh(CGMainDisplayID()));
#else
    const int screen_width_px = 1920;
    const int screen_height_px = 1080;
#endif
    const size_t n_env = envIds.size();
    std::vector<int> widths, heights, pos_x, pos_y;
    widths.resize(n_env);
    heights.resize(n_env);
    pos_x.resize(n_env);
    pos_y.resize(n_env);
    if (n_env == 1) {
      widths[0] = screen_width_px;
      heights[0] = screen_height_px;
      pos_x[0] = 0;
      pos_y[0] = 0;
    } else if (n_env > 1) {
      // Random orientation: 0=columns, 1=rows
      const int orient = Utils::GetRandomValue<int>(0, 1);
      std::vector<int> weights(n_env);
      int sumw = 0;
      for (size_t i = 0; i < n_env; ++i) {
        weights[i] = Utils::GetRandomValue<int>(1, 100);
        sumw += weights[i];
      }
      if (orient == 0) {
        int acc = 0;
        for (size_t i = 0; i < n_env; ++i) {
          widths[i] = static_cast<int>(
              (static_cast<int64_t>(weights[i]) * screen_width_px) / sumw);
          heights[i] = screen_height_px;
          pos_x[i] = acc;
          pos_y[i] = 0;
          acc += widths[i];
        }
        if (acc != screen_width_px)
          widths.back() += (screen_width_px - acc);
      } else {
        int acc = 0;
        for (size_t i = 0; i < n_env; ++i) {
          heights[i] = static_cast<int>(
              (static_cast<int64_t>(weights[i]) * screen_height_px) / sumw);
          widths[i] = screen_width_px;
          pos_x[i] = 0;
          pos_y[i] = acc;
          acc += heights[i];
        }
        if (acc != screen_height_px)
          heights.back() += (screen_height_px - acc);
      }
    }

    size_t _idx = 0;
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
      args.PushBack(rapidjson::Value()
                        .SetString("--no-first-run", doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());
      args.PushBack(
          rapidjson::Value()
              .SetString("--no-default-browser-check", doc.GetAllocator())
              .Move(),
          doc.GetAllocator());
      // Use the precomputed tiled size/position for this index.
      int w = widths[_idx];
      int h = heights[_idx];
      int x = pos_x[_idx];
      int y = pos_y[_idx];
      char window_position_arg[128] = {};
      char window_size_arg[128] = {};
      int llen = std::snprintf(window_position_arg, sizeof(window_position_arg),
                               "--window-position=%d,%d", x, y);
      (void)llen;
      llen = std::snprintf(window_size_arg, sizeof(window_size_arg),
                           "--window-size=%d,%d", w, h);
      (void)llen;
      args.PushBack(rapidjson::Value()
                        .SetString(window_position_arg, doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());

      args.PushBack(rapidjson::Value()
                        .SetString(window_size_arg, doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());
      ++_idx;

      env.AddMember(
          rapidjson::Value().SetString("args", doc.GetAllocator()).Move(),
          args.Move(), doc.GetAllocator());
      urls.PushBack(rapidjson::Value()
                        .SetString("https://myip.ipipv.com", doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());
      urls.PushBack(rapidjson::Value()
                        .SetString("https://www.baidu.com", doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());
      urls.PushBack(rapidjson::Value()
                        .SetString("https://www.bing.com", doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());
      env.AddMember(
          rapidjson::Value().SetString("urls", doc.GetAllocator()).Move(),
          urls.Move(), doc.GetAllocator());
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
  bool SDKWrapper::GenOpenBrwReqAuto(const size_t &count,
                                   std::set<uint64_t> &envIds,
                                   std::string &reqBody) {
  bool result = false;
  envIds.clear();
  reqBody.clear();
  do {
    if (count <= 0)
      break;
    for (size_t i = 0; i < count; ++i) {
      auto r = BrowserEnvCreate(
          YunLoginChromiumVersions[Utils::GetRandomValue<size_t>(
              0, YunLoginChromiumVersions.size() - 1)],"","");
      if (r > 0) {
        envIds.insert(r);
      }
    }
    if (envIds.empty())
      break;
    rapidjson::Document doc(rapidjson::Type::kObjectType);
    rapidjson::Value envs(rapidjson::Type::kArrayType);

    // Tile windows to exactly fill the primary display in logical pixels.
  #if defined(IS_WINDOWS)
    const int screen_width_px = GetSystemMetrics(SM_CXSCREEN);
    const int screen_height_px = GetSystemMetrics(SM_CYSCREEN);
  #elif defined(IS_APPLE)
    const int screen_width_px = static_cast<int>(CGDisplayPixelsWide(CGMainDisplayID()));
    const int screen_height_px = static_cast<int>(CGDisplayPixelsHigh(CGMainDisplayID()));
  #else
    const int screen_width_px = 1920;
    const int screen_height_px = 1080;
  #endif
    const size_t n_env = envIds.size();
    std::vector<int> widths, heights, pos_x, pos_y;
    widths.resize(n_env);
    heights.resize(n_env);
    pos_x.resize(n_env);
    pos_y.resize(n_env);
      if (n_env == 1) {
        widths[0] = screen_width_px;
        heights[0] = screen_height_px;
      pos_x[0] = 0;
      pos_y[0] = 0;
    } else if (n_env > 1) {
      // Random orientation: 0=columns, 1=rows
      const int orient = Utils::GetRandomValue<int>(0, 1);
      std::vector<int> weights(n_env);
      int sumw = 0;
      for (size_t i = 0; i < n_env; ++i) {
        weights[i] = Utils::GetRandomValue<int>(1, 100);
        sumw += weights[i];
      }
      if (orient == 0) {
        int acc = 0;
        for (size_t i = 0; i < n_env; ++i) {
          widths[i] = static_cast<int>(
              (static_cast<int64_t>(weights[i]) * screen_width_px) / sumw);
          heights[i] = screen_height_px;
          pos_x[i] = acc;
          pos_y[i] = 0;
          acc += widths[i];
        }
        if (acc != screen_width_px) widths.back() += (screen_width_px - acc);
      } else {
        int acc = 0;
        for (size_t i = 0; i < n_env; ++i) {
          heights[i] = static_cast<int>(
              (static_cast<int64_t>(weights[i]) * screen_height_px) / sumw);
          widths[i] = screen_width_px;
          pos_x[i] = 0;
          pos_y[i] = acc;
          acc += heights[i];
        }
        if (acc != screen_height_px) heights.back() += (screen_height_px - acc);
      }
    }

    size_t _idx = 0;
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
        args.PushBack(rapidjson::Value()
                .SetString("--no-first-run", doc.GetAllocator())
                .Move(),
              doc.GetAllocator());
      args.PushBack(
          rapidjson::Value()
              .SetString("--no-default-browser-check", doc.GetAllocator())
              .Move(),
          doc.GetAllocator());
        // Use the precomputed tiled size/position for this index.
        int w = widths[_idx];
        int h = heights[_idx];
        int x = pos_x[_idx];
        int y = pos_y[_idx];
        char window_position_arg[128] = {};
        char window_size_arg[128] = {};
        int llen = std::snprintf(window_position_arg, sizeof(window_position_arg),
               "--window-position=%d,%d", x, y);
        (void)llen;
        llen = std::snprintf(window_size_arg, sizeof(window_size_arg),
                 "--window-size=%d,%d", w, h);
        (void)llen;
        args.PushBack(rapidjson::Value()
                .SetString(window_position_arg, doc.GetAllocator())
                .Move(),
              doc.GetAllocator());

        args.PushBack(rapidjson::Value()
                .SetString(window_size_arg, doc.GetAllocator())
                .Move(),
              doc.GetAllocator());
        ++_idx;

      env.AddMember(
          rapidjson::Value().SetString("args", doc.GetAllocator()).Move(),
          args.Move(), doc.GetAllocator());
      urls.PushBack(rapidjson::Value()
                        .SetString("https://myip.ipipv.com", doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());
      urls.PushBack(rapidjson::Value()
                        .SetString("https://www.baidu.com", doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());
      urls.PushBack(rapidjson::Value()
                        .SetString("https://www.bing.com", doc.GetAllocator())
                        .Move(),
                    doc.GetAllocator());
      env.AddMember(
          rapidjson::Value().SetString("urls", doc.GetAllocator()).Move(),
          urls.Move(), doc.GetAllocator());
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
void SDKWrapper::AutoProcess() {
  std::set<uint64_t> AllEnvIds;
  do {
    do {
      if (!auto_test_running_.load())
        break;

      //{
      //  std::lock_guard<std::mutex> lk(mtx_deldir_);
      //  for (auto it = deldirs_.begin(); it != deldirs_.end();) {
      //   if (Utils::RemoveDir(*it)) {
      //      it = deldirs_.erase(it);
      //     continue;
      //    }
      //   ++it;
      //  }
      //}

      const size_t count = Utils::GetRandomValue(10, 20);
      std::set<uint64_t> envIds;
      std::string reqBody;
      if (!GenOpenBrwReqAuto2(count, envIds, reqBody) || envIds.empty())
        break;

      // Clear per-batch tracking sets before issuing the open request.
      {
        std::lock_guard<std::mutex> lk(notify_mtx_);
        opened_envs_.clear();
        open_done_envs_.clear();
        closed_envs_.clear();
        close_done_envs_.clear();
      }

      // --- Open phase ---
      BrowserOpen(reqBody.c_str(), reqBody.size());
      // Wait until every env in this batch has finished opening
      // (success or failure), up to 15 s per browser.
      const int openTimeout = static_cast<int>(envIds.size() * 15);
      WaitForEnvEvents(envIds, open_done_envs_, openTimeout);

      // Compute the subset that opened successfully (eligible for close).
      std::set<uint64_t> toClose;
      {
        std::lock_guard<std::mutex> lk(notify_mtx_);
        for (auto id : envIds) {
          if (opened_envs_.count(id))
            toClose.insert(id);
        }
      }

      // Wait 5 seconds so the user can observe the opened browsers.
      std::this_thread::sleep_for(std::chrono::seconds(5));

      // --- Close phase ---
      if (!toClose.empty()) {
        GenCloseBrwReq(toClose, reqBody);
        BrowserClose(reqBody.c_str(), reqBody.size());
        const int closeTimeout = static_cast<int>(toClose.size() * 15);
        WaitForEnvEvents(toClose, close_done_envs_, closeTimeout);
      }

    } while (0);

    if (!ready_.load())
      break;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  } while (true);
}