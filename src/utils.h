#if !defined(__SDK_DEMO_UTILS_H__)
#define __SDK_DEMO_UTILS_H__

class Utils {
public:
  explicit Utils() = default;
  ~Utils() = default;

public:
  static std::string JsonToString(const rapidjson::Value &valObj);
  static std::string PrettyJson(const std::string &json);
  static stl::path GetCurrentProcessDir();
  static std::vector<std::string> SplitCommandLine(const std::string &input);
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
  static bool StringToBool(const std::string &input, bool *value);
  static bool TryParseInt(const std::string &input, int *value);
  static std::string ToLowerCopy(std::string input);
};

#endif //__SDK_DEMO_UTILS_H__
