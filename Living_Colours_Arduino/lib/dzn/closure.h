#ifndef DZN_CLOSURE_H
#define DZN_CLOSURE_H

#include <dzn/config.h>

#if DZN_DYNAMIC_QUEUES

typedef struct {
  void (*func)(void*);
  void *args;
} dzn_closure;

#else // !DZN_DYNAMIC_QUEUES

typedef struct {
  void (*func)(void*);
  char args[DZN_MAX_ARGS_SIZE];
} dzn_closure;

#endif // !DZN_DYNAMIC_QUEUES

#endif /* DZN_CLOSURE_H */
