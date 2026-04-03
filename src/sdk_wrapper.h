#if !defined(__SDK_WRAPPER_H__)
#define __SDK_WRAPPER_H__

class SDKWrapper {
public:
  explicit SDKWrapper();
  ~SDKWrapper();

private:
  void Init();
  void UnInit();

private:
  ISDK *brosdk_ = nullptr;
};

#endif //__SDK_WRAPPER_H__