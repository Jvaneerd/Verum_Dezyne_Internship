#ifndef DZN_LOCATOR_H
#define DZN_LOCATOR_H

#include <dzn/config.h>

#if DZN_LOCATOR_SERVICES
#include <dzn/map.h>
#endif // !DZN_LOCATOR_SERVICES

#include <dzn/runtime.h>

typedef struct locator locator;
struct locator {
	runtime* rt;
	void (*illegal)();
#if DZN_LOCATOR_SERVICES
  	map services;
#endif // DZN_LOCATOR_SERVICES
};

void locator_init(locator* self, runtime* rt);
locator* locator_clone(locator* self);
#if DZN_LOCATOR_SERVICES
void* locator_get(locator* self, char* key);
locator* locator_set(locator* self, char* key, void* value);
#endif // DZN_LOCATOR_SERVICES

#endif // DZN_LOCATOR_H
