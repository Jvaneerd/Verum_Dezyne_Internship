#ifndef DZN_RUNTIME_H
#define DZN_RUNTIME_H

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#include <dzn/meta.h>
#if DZN_TRACING
#include <stdio.h>
#endif // DZN_TRACING

#include <dzn/queue.h>

#include "Arduino.h"

#define PIN_ILLEGAL 3
#define PIN_DEBUG 4

typedef struct {
	uint8_t dummy;
} runtime;

typedef struct locator locator;
typedef struct runtime_info runtime_info;
struct runtime_info {
  runtime* rt;
  locator* locator;
  bool handling;
  bool performs_flush;
  runtime_info* deferred;
  queue q;
};

typedef struct {
#if DZN_TRACING
  dzn_meta_t dzn_meta;
#endif // !DZN_TRACING
  runtime_info dzn_info;
} component;

void runtime_init (runtime*);
void runtime_illegal_handler();
void runtime_info_init (runtime_info* info, locator* loc);
void runtime_flush (runtime_info* self);
void runtime_defer (void* src, void* tgt, void (*event)(void*), void* args);
void runtime_event (void (*event)(void*), void* args);
void runtime_start (runtime_info* info);
void runtime_finish (runtime_info* info);


#if DZN_TRACING

char* _bool_to_string (bool b);
bool string_to__bool (char *s);
char* _int_to_string (int8_t i);
int8_t string_to__int (char *s);
#define DZN_TRACE(msg) fprintf (stderr, "%s\n", msg)
char* runtime_path (dzn_meta_t const* m, char* p);
void runtime_trace_in (dzn_port_meta_t const* m, char const* e);
void runtime_trace_out (dzn_port_meta_t const* m, char const* e);
#define RUNTIME_TRACE_in runtime_trace_in
#define RUNTIME_TRACE_out runtime_trace_out

#else // !DZN_TRACING

#define DZN_TRACE(msg)
#define RUNTIME_TRACE_in(m,e)
#define RUNTIME_TRACE_out(m,e)

#endif // !DZN_TRACING

#endif /* DZN_RUNTIME_H */
