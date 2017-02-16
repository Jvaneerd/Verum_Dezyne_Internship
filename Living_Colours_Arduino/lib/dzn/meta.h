#ifndef DZN_META_H
#define DZN_META_H

#include <dzn/config.h>

#define DZN_CONNECT(provided, required)               \
{                                                     \
  required->meta.provides = provided->meta.provides;  \
  provided->meta.requires = required->meta.requires;  \
  provided->out = required->out;                      \
  required->in = provided->in;                        \
}

#if DZN_TRACING
typedef struct dzn_meta {
  char const* name;
  struct dzn_meta const* parent;
} dzn_meta_t;
#endif // DZN_TRACING

typedef struct {
  struct {
    void* address;
#if DZN_TRACING
    char const* port;
    dzn_meta_t const* meta;
#endif // DZN_TRACING
  } provides;
  struct {
    void* address;
#if DZN_TRACING
    char const* port;
    dzn_meta_t const* meta;
#endif // DZN_TRACING
  } requires;
} dzn_port_meta_t;

#endif // DZN_META_H
