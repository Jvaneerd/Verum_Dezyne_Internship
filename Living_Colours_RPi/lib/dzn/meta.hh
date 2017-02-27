#ifndef DZN_META_HH
#define DZN_META_HH

#include <cassert>
#include <functional>
#include <string>
#include <stdexcept>
#include <vector>

inline std::string to_string()
{
  return "return";
}

namespace dzn
{
  struct meta;

  namespace port
  {
    struct meta
    {
      struct
      {
        std::string port;
        void* address;
        const dzn::meta* meta;
      } provides;

      struct
      {
        std::string port;
        void* address;
        const dzn::meta* meta;
      } requires;
    };
  }

  struct meta
  {
    std::string name;
    std::string type;
    const meta* parent;
    std::vector<const meta*> children;
    std::vector<std::function<void()>> ports_connected;
  };

  struct illegal_handler {std::function<void()> illegal = [] {assert(!"h:illegal");};};

  inline std::string path(const meta* m, std::string p = std::string())
  {
    p = p.empty() ? p : "." + p;
    if(!m) return "<external>" + p;
    if(!m->parent) return m->name + p;
    return path(m->parent, m->name + p);
  }

  struct binding_error: public std::runtime_error
  {
    binding_error(const port::meta& m, const std::string& msg)
    : std::runtime_error("not connected: " + path(m.provides.address ? m.provides.meta : m.requires.meta,
                                                  m.provides.address ? m.provides.port : m.requires.port) + "." + msg)
    {}
  };
}
#endif //DZN_META_HH
