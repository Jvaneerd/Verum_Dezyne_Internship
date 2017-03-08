#include <dzn/locator.hh>
#include <dzn/pump.hh>

#include <algorithm>
#include <cassert>

#ifdef DEBUG_RUNTIME
#include <iostream>
#endif

namespace dzn
{
  void port_block(const locator& l, void* p)
  {
    l.get<dzn::pump>().block(p);
  }
  void port_release(const locator& l, void* p, std::function<void()>& out_binding)
  {
    if(out_binding) out_binding();
    out_binding = nullptr;
    l.get<dzn::pump>().release(p);
  }
  static void debug(const std::string& s)
  {
#ifdef DEBUG_RUNTIME
    std::cout << s << std::endl;
#endif
  }

  static void debug(const std::string& s, int id)
  {
#ifdef DEBUG_RUNTIME
    std::cout << '[' << id << "] " << s << std::endl;
#endif
  }

  static std::list<coroutine>::iterator find_self(std::list<coroutine>& coroutines)
  {
    assert(1 == std::count_if(coroutines.begin(), coroutines.end(), [](const coroutine& c){return c.port == nullptr && !c.finished;}));
    auto self = std::find_if(coroutines.begin(), coroutines.end(), [](dzn::coroutine& c){return c.port == nullptr && !c.finished;});
    return self;
  }

  static std::list<coroutine>::iterator find_blocked(std::list<coroutine>& coroutines, void* port)
  {
    auto self = std::find_if(coroutines.begin(), coroutines.end(), [port](dzn::coroutine& c){return c.port == port;});
    return self;
  }

  static void finish(std::list<coroutine>& coroutines)
  {
    auto self = find_self(coroutines);
    self->finished = true;
    debug("finish coroutine", self->id);
  }

  int coroutine::g_id = 0;

  pump::pump()
  : collateral_block_lambda([this]{collateral_block();})
  , switch_context()
  , running(true)
  , task(std::async(std::launch::async, std::ref(*this)))
  {}
  pump::~pump()
  {
    std::unique_lock<std::mutex> lock(mutex);
    running = false;
    condition.notify_one();
    if (lock) lock.unlock();
    task.wait();
  }
  void pump::wait()
  {
    std::unique_lock<std::mutex> lock(mutex);
    idle.wait(lock, [this]{return queue.empty();});
  }
  void pump::operator()()
  {
    try
    {
      thread_id = std::this_thread::get_id();

      worker = [&] {
        std::unique_lock<std::mutex> lock(mutex);
        if(queue.empty())
        {
          idle.notify_one();
        }
        if(timers.empty())
        {
          condition.wait(lock, [this]{return queue.size() || !running;});
        }
        else
        {
          condition.wait_until(lock, timers.begin()->first.t, [this]{return queue.size() || !running;});
        }

        while(timers.size() && timers.begin()->first.expired())
        {
          auto t = *timers.begin();
          timers.erase(timers.begin());
          if (lock) lock.unlock();
          t.second();
        }

        if(queue.size())
        {
          std::function<void()> f(std::move(queue.front()));
          queue.pop();
          if (lock) lock.unlock();
          f();
        }
      };

      coroutine zero;
      create_context();

      exit = [&]{debug("enter exit"); zero.release();};

      std::unique_lock<std::mutex> lock(mutex);
      while(running || queue.size() || collateral_blocked.size())
      {
        assert(coroutines.size());
        if (lock) lock.unlock();
        coroutines.back().call(zero);
        lock.lock();
        coroutines.remove_if([](dzn::coroutine& c){if(c.finished) debug("removing", c.id); return c.finished;});
      }
      debug("finish pump");
      assert(queue.empty());
    }
    catch(const std::exception& e)
    {
#ifdef DEBUG_RUNTIME
      std::cout << "oops: " << e.what() << std::endl;
#endif
      std::terminate();
    }
  }
  void pump::create_context()
  {
    coroutines.emplace_back([&]{
        try
        {
          auto self = find_self(coroutines);
          debug("create context", self->id);
          while((running || queue.size()) && !self->released)
          {
            worker();
            if(!self->released)
            {
              collateral_release(self);
            }
          }

          if(self->released) finish(coroutines);

          if(switch_context) decltype(switch_context)(std::move(switch_context))();

          if(!self->released) collateral_release(self);

          exit();
        }
#if HAVE_BOOST_COROUTINE
        catch(const boost::coroutines::detail::forced_unwind&) {throw;}
#else
        catch(const context::unwind&) {}
#endif
        catch(const std::exception& e)
        {
#ifdef DEBUG_RUNTIME
          std::cout << "oops: " << e.what() << std::endl;
#endif
          std::terminate();
        }
      });
  }
  void pump::collateral_block()
  {
    auto self = find_self(coroutines);
    debug("collateral_block", self->id);

    collateral_blocked.splice(collateral_blocked.end(), coroutines, self);
    create_context();
    self->yield_to(coroutines.back());

    debug("collateral_unblock", self->id);
  }
  void pump::collateral_release(std::list<coroutine>::iterator self)
  {
    if(collateral_blocked.size()) finish(coroutines);
    while(collateral_blocked.size())
    {
      coroutines.splice(coroutines.end(), collateral_blocked, collateral_blocked.begin());
      self->yield_to(coroutines.back());
    }
  }
  void pump::block(void* p)
  {
    auto it = skip_block.find(p);
    if(it != skip_block.end())
    {
      skip_block.erase(it);
      return;
    }

    auto self = find_self(coroutines);
#if 0 //DEAD CODE? verify and remove .skip_block member
    if(self->skip_block)
    {
      self->skip_block = false;
      return;
    }
#endif

    self->port = p;

    debug("block", self->id);

    create_context();

    self->yield_to(coroutines.back());
    debug("entered context", self->id);
#ifdef DEBUG_RUNTIME
    std::cout << "routines: ";
    for (auto& c: coroutines) {
      std::clog << c.id << " ";
    }
    std::cout << std::endl;
#endif
    coroutines.remove_if([](dzn::coroutine& c){if(c.finished) debug("removing",c.id); return c.finished;});
  }
  void pump::release(void* p)
  {
    auto self = find_self(coroutines);

    auto blocked = find_blocked(coroutines, p);
    if(blocked == coroutines.end())
    {
    debug("skip block", self->id);
      skip_block.insert(p);
      return;
    }

    debug("unblock", blocked->id);
    debug("released", self->id);
    self->released = true;

    switch_context = [blocked,self] {
      blocked->port = nullptr;

      debug("switch from", self->id);
      debug("to", blocked->id);

      self->yield_to(*blocked);
    };
  }
  void pump::operator()(const std::function<void()>& e)
  {
    assert(e);
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(e);
    condition.notify_one();
  }
  void pump::operator()(std::function<void()>&& e)
  {
    assert(e);
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(std::move(e));
    condition.notify_one();
  }
  void pump::handle(size_t id, size_t ms, const std::function<void()>& e)
  {
    assert(e);
    assert(std::find_if(timers.begin(), timers.end(), [id](const std::pair<deadline, std::function<void()>>& p){ return p.first.id == id; }) == timers.end());
    timers.emplace(deadline(id, ms), e);
  }
  void pump::remove(size_t id)
  {
    auto it = std::find_if(timers.begin(), timers.end(), [id](const std::pair<deadline, std::function<void()>>& p){ return p.first.id == id; });
    if(it != timers.end()) timers.erase(it);
  }
}
