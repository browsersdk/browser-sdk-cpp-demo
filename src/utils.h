#if !defined(__SDK_DEMO_UTILS_H__)
#define __SDK_DEMO_UTILS_H__

class Utils {
public:
  explicit Utils() = default;
  ~Utils() = default;

public:
  static std::string JsonToString(const rapidjson::Value &valObj);
  static stl::path GetCurrentProcessDir();
  static void MainProc(const std::function<void(const std::string &input,
                                                bool &exit_flag)> &callback_);
  template <typename T> static T GetRandomValue(const T &a, const T &b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    if constexpr (std::is_floating_point<T>::value) {
      std::uniform_real_distribution<> dis(static_cast<double>(a),
                                           static_cast<double>(b));
      return static_cast<T>(dis(gen));
    } else if constexpr (std::is_integral<T>::value) {
      std::uniform_int_distribution<T> dis(static_cast<T>(a),
                                           static_cast<T>(b));
      return dis(gen);
    }
    return T();
  }
  static std::string Time();
  static uint64_t TimeStamp();
  static std::string ReadFile(const stl::path &,
                              const int &mode = std::ios::in |
                                                std::ios::binary);
  static bool RemoveDir(const stl::path &dir);
  static stl::path GetAppDataDir();
};

#endif //__SDK_DEMO_UTILS_H__