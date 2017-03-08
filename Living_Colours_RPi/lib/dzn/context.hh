#ifndef DZN_CONTEXT_HH
#define DZN_CONTEXT_HH

#include <cassert>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>

namespace dzn
{
class context
{
  enum State {INITIAL, RELEASED, BLOCKED, FINAL};
  inline std::string to_string(State state)
  {
    switch(state)
    {
    case INITIAL: return "INITIAL";
    case RELEASED: return "RELEASED";
    case BLOCKED: return "BLOCKED";
    case FINAL: return "FINAL";
    }
    throw std::logic_error("UNKNOWN STATE");
  }
  State state;
  std::function<void()> rel;
  std::function<void(std::function<void(context&)>&)> work;
  std::mutex mutex;
  std::condition_variable condition;
  std::thread thread;
public:
  struct unwind: public std::runtime_error
  {
    unwind(): std::runtime_error("unwind") {}
  };
  context()
  : state(INITIAL)
  , work()
  , mutex()
  , condition()
  , thread([this] {
      try
      {
        std::unique_lock<std::mutex> lock(mutex);
        while(state != FINAL)
        {
          do_block(lock);
          if(state == FINAL) break;
          if(!this->work) break;
          lock.unlock();
          std::function<void(context&)> yield([this](context& c){ this->yield(c); });
          assert(yield);
          this->work(yield);
          lock.lock();
          if(state == FINAL) break;
          if(this->rel) this->rel();
        }
      }
      catch(const unwind&){}
    })
  {
    std::unique_lock<std::mutex> lock(mutex);
    while(state != BLOCKED) condition.wait(lock);
  }
  context(bool)
  : state(INITIAL)
  , work()
  , mutex()
  , condition()
  , thread()
  {}
  context(context&&) = delete;
  context& operator=(context&&) = delete;
  context(const context&) = delete;
  context& operator=(const context&) = delete;
  template <typename Work>
  context(Work&& work)
  : context()
  {
    this->work = std::move(work);
  }
  ~context()
  {
    std::unique_lock<std::mutex> lock(mutex);
    do_finish(lock);
  }
  std::thread::id get_id()
  {
    return thread.get_id();
  }
  void finish()
  {
    std::unique_lock<std::mutex> lock(mutex);
    do_finish(lock);
  }
  void block()
  {
    std::unique_lock<std::mutex> lock(mutex);
    do_block(lock);
  }
  void release()
  {
    std::unique_lock<std::mutex> lock(mutex);
    do_release(lock);
  }
  void call(context& c)
  {
    std::unique_lock<std::mutex> lock(mutex);
    do_release(lock);

    std::unique_lock<std::mutex> lock2(c.mutex);
    c.state = BLOCKED;

    lock.unlock();

    do { c.condition.wait(lock2); } while(c.state == BLOCKED);
  }
  template <typename Work>
  void yield(Work&& work, context& c)
  {
    std::unique_lock<std::mutex> lock(mutex);
    this->work = std::move(work);
    this->rel = [&]{c.release();};
    do_release(lock);

    std::unique_lock<std::mutex> lock2(c.mutex);
    c.state = BLOCKED;

    lock.unlock();

    do { c.condition.wait(lock2); } while(c.state == BLOCKED);
  }
  void yield(context& to)
  {
    if(&to == this) return;
    std::unique_lock<std::mutex> lock(mutex);
    to.release();
    do_block(lock);
  }
private:
  void do_block(std::unique_lock<std::mutex>& lock)
  {
    state = BLOCKED;
    condition.notify_one();
    do { condition.wait(lock); } while(state == BLOCKED);
    if(state == FINAL) throw unwind();
  }
  void do_release(std::unique_lock<std::mutex>&)
  {
    if(state != BLOCKED)
      throw std::runtime_error("not allowed to release a call which is " +
                               to_string(state));
    state = RELEASED;
    condition.notify_one();
  }
  void do_finish(std::unique_lock<std::mutex>& lock)
  {
    state = FINAL;
    lock.unlock();
    condition.notify_all();
    if(thread.joinable()) thread.join();
  }
};
}

#endif //DZN_CONTEXT_HH
