#include <dzn/locator.h>

#if DZN_LOCATOR_SERVICES
#include <stdlib.h>
#endif // DZN_LOCATOR_SERVICES

void locator_init(locator* self, runtime* rt) {
  self->rt = rt;
  self->illegal = runtime_illegal_handler;
#if defined(DZN_LOCATOR_SERVICES)
  map_init (&self->services);
#endif
}

#if DZN_LOCATOR_SERVICES
int map_copy(map_element* elt, void* dst) {
  map* m = dst;
  return map_put (m, elt->key, elt->data);
}

void* locator_get(locator* self, char* key) {
  void* p = 0;
  map_get (&self->services, key, &p);
  return p;
}

locator* locator_set(locator* self, char* key, void* value) {
  map_put (&self->services, key, value);
  return self;
}
#endif // DZN_LOCATOR_SERVICES

locator* locator_clone(locator* self) {
#if DZN_LOCATOR_SERVICES
  locator* clone = malloc(sizeof(locator));
  clone->rt = self->rt;
  map_init (&clone->services);
  map_iterate(&self->services, map_copy, &clone->services);
  return clone;
#else // !DZN_LOCATOR_SERVICES
  return self;
#endif // !DZN_LOCATOR_SERVICES
}
