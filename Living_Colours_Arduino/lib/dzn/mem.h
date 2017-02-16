#ifndef DZN_MEM_H
#define DZN_MEM_H

#include <dzn/config.h>
#if DZN_DYNAMIC_QUEUES || DZN_LOCATOR_SERVICES
#include <stdlib.h>

void* dzn_calloc (size_t n, size_t size);
void* dzn_malloc (size_t size);
#endif // DZN_DYNAMIC_QUEUES || DZN_LOCATOR_SERVICES

#endif /* DZN_MEM_H */
