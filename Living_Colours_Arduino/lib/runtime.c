#include <dzn/runtime.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <dzn/mem.h>
#include <dzn/closure.h>
#include <dzn/locator.h>
#include <dzn/queue.h>

typedef struct {
  uint8_t size;
  void (*f)(void*);
  void* self;
} arguments;

void
runtime_illegal_handler()
{
  digitalWrite(PIN_DEBUG, HIGH);
  assert(!"illegal");
}

void
runtime_init (runtime* self)
{
}

void
runtime_info_init (runtime_info* self, locator* loc)
{
  self->rt = loc->rt;
  self->locator = loc;
  self->handling = false;
  self->performs_flush = false;
  self->deferred = 0;
  queue_init(&self->q);
}

static void runtime_handle_event (runtime_info* info, void (*event)(void*), void* args);

void
runtime_flush (runtime_info* info)
{
  queue* q = &info->q;
  while (!queue_empty (q))
  {
#if DZN_DYNAMIC_QUEUES
    dzn_closure* c = queue_pop (q);
    runtime_handle_event (info, c->func, c->args);
    free (c->args);
    free (c);
#else // !DZN_DYNAMIC_QUEUES
    dzn_closure c = *(dzn_closure*)queue_pop (q);
    runtime_handle_event (info, c.func, c.args);
#endif // !DZN_DYNAMIC_QUEUES
  }
  if (info->deferred)
  {
    runtime_info* tgt = info->deferred;
    info->deferred = 0;
    if (tgt && !tgt->handling)
      runtime_flush (tgt);
  }
}

void
runtime_defer (void* vsrc, void* vtgt, void (*event)(void*), void* args)
{
  component* csrc = vsrc;
  component* ctgt = vtgt;
  runtime_info* src = csrc?&csrc->dzn_info:0;
  runtime_info* tgt = ctgt?&ctgt->dzn_info:0;
  if (!(src && src->performs_flush) && !tgt->handling)
  {
    runtime_handle_event (tgt, event, args);
    return;
  }
#if DZN_DYNAMIC_QUEUES
  dzn_closure *c = dzn_malloc (sizeof (dzn_closure));
  c->func = event;
  arguments *a = args;
  c->args = dzn_malloc (a->size);
  memcpy (c->args, a, a->size);
  queue_push (&tgt->q, c);
  if (src)
    src->deferred = tgt;
#else // !DZN_DYNAMIC_QUEUES
  dzn_closure c;
  c.func = event;
  arguments *a = args;
  assert(a->size <= DZN_MAX_ARGS_SIZE);
  memcpy(&c.args, a, a->size);
  queue_push (&tgt->q, &c);
  if (src)
    src->deferred = tgt;
#endif // !DZN_DYNAMIC_QUEUES
}

static void
runtime_handle_event (runtime_info* info, void (*event)(void*), void* args)
{
  runtime_start(info);
  event(args);
  runtime_finish(info);
}

void
runtime_event (void (*event)(void*), void* args)
{
  arguments* a = args;
  component* c = a->self;
  runtime_handle_event (&c->dzn_info, event, args);
}

void
runtime_start (runtime_info* info)
{
  if (!info->handling)
  {
    info->handling = true;
  }
  else
  {
    assert(!"component already handling an event");
  }
}

void
runtime_finish (runtime_info* info)
{
    info->handling = false;
    runtime_flush (info);
}

#if DZN_TRACING
char*
runtime_path (dzn_meta_t const* m, char* p)
{
  char buf[1024] = "";
  strcpy (buf, m ? m->name : "<external>");
  if (*p) {
    strcat (buf, ".");
    strcat (buf, p);
  }
  strcpy (p, buf);
  if (!m || !m->parent) return p;
  return runtime_path (m->parent, p);
}

void
runtime_trace_in (dzn_port_meta_t const* meta, char const* e)
{
  char pbuf[1024] = "";
  char rbuf[1024] = "";
  strcpy(pbuf, meta->provides.port);
  strcpy(rbuf, meta->requires.port);
  fprintf (stderr, "%s.%s -> %s.%s\n",
           runtime_path (meta->requires.meta, rbuf), e,
           runtime_path (meta->provides.meta, pbuf), e);
}

void
runtime_trace_out (dzn_port_meta_t const* meta, char const* e)
{
  char pbuf[1024] = "";
  char rbuf[1024] = "";
  strcpy(pbuf, meta->provides.port);
  strcpy(rbuf, meta->requires.port);
  fprintf (stderr, "%s.%s -> %s.%s\n",
           runtime_path (meta->provides.meta, pbuf), e,
           runtime_path (meta->requires.meta, rbuf), e);
}

char*
_bool_to_string (bool b)
{
  return b ? "true" : "false";
}

bool
string_to__bool (char *s)
{
  return !strcmp (s, "true");
}

char*
_int_to_string (int8_t i)
{
  static char buf[sizeof (i) * 2 + 1];
  sprintf (buf, "%d", i);
  return buf;
}

int8_t
string_to__int (char *s)
{
  return atoi (s);
}
#endif // !DZN_TRACING
