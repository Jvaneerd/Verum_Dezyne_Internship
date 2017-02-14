#include <dzn/mem.h>

#include <assert.h>

#if DZN_DYNAMIC_QUEUES || DZN_LOCATOR_SERVICES
void*
dzn_calloc (size_t n, size_t size)
{
  void* res = calloc(n, size);
  if (!res) assert (0);
  return res;
}

void*
dzn_malloc(size_t size)
{
  return dzn_calloc(1, size);
}
#endif // DZN_DYNAMIC_QUEUES || DZN_LOCATOR_SERVICES
