#ifndef DZN_CONFIG_H
#define DZN_CONFIG_H

#ifndef DZN_TINY
#define DZN_TINY 0
#endif // DZN_TINY

#if !DZN_TINY

#define DZN_ILLEGAL(self) self->dzn_info.locator->illegal()

#ifndef DZN_TRACING
#define DZN_TRACING 1
#endif

#ifndef DZN_DYNAMIC_QUEUES
#define DZN_DYNAMIC_QUEUES 1
#endif

#ifndef DZN_LOCATOR_SERVICES
#define DZN_LOCATOR_SERVICES 1
#endif

#else // DZN_TINY

#define DZN_ILLEGAL(self) while (0)

#endif // DZN_TINY

#if !DZN_DYNAMIC_QUEUES
#define DZN_MAX_ARGS_SIZE 24
#define DZN_QUEUE_SIZE 7
#endif // !DZN_DYNAMIC_QUEUES

#endif /* DZN_CONFIG_H */
