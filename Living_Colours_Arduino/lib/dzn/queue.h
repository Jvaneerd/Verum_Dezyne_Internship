#ifndef DZN_QUEUE_H
#define DZN_QUEUE_H

#include <dzn/config.h>

#include <stdbool.h>
#include <stdint.h>

#if DZN_DYNAMIC_QUEUES
typedef struct Node {
    void* item;
    struct Node* next;
} Node;
#else // !DZN_DYNAMIC_QUEUES
#include <dzn/closure.h>
typedef struct Node {
    dzn_closure item;
} Node;
#endif // !DZN_DYNAMIC_QUEUES

typedef struct {
    Node* head;
    Node* tail;
    uint8_t size;
#if !DZN_DYNAMIC_QUEUES
    Node element[DZN_QUEUE_SIZE];
#endif // !DZN_DYNAMIC_QUEUES
} queue;

void queue_init(queue*);
bool queue_empty (queue*);
void queue_push (queue*, void*);
uint8_t queue_size (queue*);
void* queue_front (queue*);
void* queue_pop (queue*);

#endif /* DZN_QUEUE_H */
