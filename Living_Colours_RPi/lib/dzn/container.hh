#ifndef DZN_CONTAINER_HH
#define DZN_CONTAINER_HH

#include <dzn/locator.hh>
#include <dzn/runtime.hh>
#include <dzn/pump.hh>

#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>

namespace dzn
{
  template <typename System>
  struct container
  {
    dzn::meta meta;
    dzn::locator locator;
    dzn::runtime runtime;
    System system;

    std::map<std::string, std::function<void()>> lookup;

    std::queue<std::string> expect;
    std::mutex mutex;
    std::condition_variable condition;

    dzn::pump pump;

    container(bool flush, dzn::locator&& l = dzn::locator{})
    : meta{"<internal>","container",0,{&system.dzn_meta},{[this]{system.check_bindings();}}}
    , locator(std::forward<dzn::locator>(l))
    , runtime()
    , system(locator.set(runtime).set(pump))
    , pump()
    {
      runtime.performs_flush(this) = flush;
      system.dzn_meta.name = "sut";
    }
    std::string match_return()
    {
      std::unique_lock<std::mutex> lock(mutex);
      condition.wait(lock, [this]{return not expect.empty();});
      std::string tmp = expect.front(); expect.pop();
      auto it = lookup.find(tmp);
      while(it != lookup.end())
      {
        it->second();
        condition.wait(lock, [this]{return not expect.empty();});
        tmp = expect.front(); expect.pop();
        it = lookup.find(tmp);
      }
      return tmp;
    }
    void match(const std::string& actual)
    {
      std::string tmp = match_return();

      if(actual != tmp)
        throw std::runtime_error("unmatched expectation: \"" + actual + "\" got: \"" + tmp + "\"");
    }
    void operator()(std::map<std::string, std::function<void()>>&& lookup, std::set<std::string>&& required_ports)
    {
      this->lookup = std::move(lookup);

      std::string port;
      std::string str;

      while(std::cin >> str)
      {
        auto it = this->lookup.find(str);
        if(it == this->lookup.end() || port.size())
        {
          std::string p = str.substr(0, str.find('.'));
          if(it == this->lookup.end() && required_ports.find(p) != required_ports.end())
          {
            if(port.empty() || port != p) port = p;
            else port.clear();
          }
          std::unique_lock<std::mutex> lock(mutex);
          condition.notify_one();
          expect.push(str);
        }
        else
        {
          pump(it->second);
          port.clear();
        }
      }
    }
  };
}

#endif //DZN_CONTAINER_HH
