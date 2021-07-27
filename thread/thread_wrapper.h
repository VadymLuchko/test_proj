#ifndef THREAD_WRAPPER_H
#define THREAD_WRAPPER_H

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

namespace thread_wrapper {

/*************************************

        Cancellation Token Class

**************************************/

class CancellationToken {
public:
  CancellationToken() : _cancelled(false) {}

  operator bool() const { return !_cancelled; }

  void cancel() { _cancelled = true; }

private:
  std::atomic<bool> _cancelled;
};

/*************************************

        Thread Wrapper Class

**************************************/

class ThreadWrapper {
protected:
  std::thread _thread;
  CancellationToken _token;

  void Reset() {
    if (!_thread.joinable())
      return;

    _token.cancel();
    _thread.join();
  }

public:
  ThreadWrapper() = default;

  template <typename Delegate, typename... Args>
  void Start(Delegate &&delegate, Args... args) {
    _thread = std::thread(delegate, args..., std::ref(_token));
  }

  void Stop() {
    std::cout << "STOP" << std::endl;
    Reset();
  }

  ~ThreadWrapper() { Reset(); }
};

/*************************************

        Test Class Using Thread Wrapper

**************************************/

class UsbHandler : public ThreadWrapper {
public:
  void Init() {
    Start([](CancellationToken &token) {
      std::cout << "START" << std::endl;
      while (token) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //....
        std::cout << "do_domething" << std::endl;
      }
      std::cout << "Cancelled" << std::endl;
    });
  }
};

void Test() {
  UsbHandler uh;
  uh.Init();

  std::this_thread::sleep_for(std::chrono::seconds(5));
  uh.Stop();
}

} // namespace thread_wrapper

#endif // THREAD_WRAPPER_H
