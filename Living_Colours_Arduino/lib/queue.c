#include <dzn/queue.h>

#include <assert.h>

#if DZN_DYNAMIC_QUEUES
#include <stdlib.h>
#include <dzn/mem.h>
#endif

void
queue_init(queue* self)
{
#if DZN_DYNAMIC_QUEUES
  self->head = 0;
  self->tail = 0;
  self->size = 0;
#else // !DZN_DYNAMIC_QUEUES
  self->head = self->element;
  self->tail = self->element;
  self->size = 0;
#endif // !DZN_DYNAMIC_QUEUES
}

bool
queue_empty (queue* self)
{
  return queue_size (self) == 0;
}

uint8_t
queue_size (queue* self)
{
  return self->size;
}

void
queue_push (queue* self, void* e)
{
#if DZN_DYNAMIC_QUEUES
  Node* n = (Node*) dzn_malloc (sizeof (Node));
  n->item = e;
  n->next = 0;

  if (!self->head) {
    self->head = n;
  } else {
    self->tail->next = n;
  }
  self->tail = n;
  self->size++;
#else // !DZN_DYNAMIC_QUEUES
  *(self->tail) = *((Node*)e);
  self->tail++;
  if (self->tail - self->element == DZN_QUEUE_SIZE) {
    self->tail = self->element;
  }
  self->size++;
  assert (self->size <= DZN_QUEUE_SIZE);
#endif // !DZN_DYNAMIC_QUEUES
}

void*
queue_pop (queue* self)
{
#if DZN_DYNAMIC_QUEUES
  assert (self->size);
  Node* head = self->head;
  void* item = head->item;
  self->head = head->next;
  self->size--;
  free (head);
  return item;
#else // !DZN_DYNAMIC_QUEUES
  assert (self->size);
  Node* res = self->head;
  self->head++;
  if (self->head - self->element == DZN_QUEUE_SIZE) {
    self->head = self->element;
  }
  self->size--;
  return res;
#endif // !DZN_DYNAMIC_QUEUES
}

void*
queue_front (queue* self)
{
#if DZN_DYNAMIC_QUEUES
  return self->head->item;
#else // !DZN_DYNAMIC_QUEUES
  return self->head;
#endif // !DZN_DYNAMIC_QUEUES
}

#ifdef QUEUE_TEST
#include <stdio.h>
int
main ()
{
  queue q = {0};
  int a = 1;
  int b = 2;
  int c = 3;
  queue_push (&q, &a);
  printf ("queue_pop a: %d\n", *(int*)queue_pop (&q));
  queue_push (&q, &a);
  queue_push (&q, &b);
  queue_push (&q, &c);
  printf ("queue_pop c: %d\n", *(int*)queue_pop (&q));
  printf ("queue_pop b: %d\n", *(int*)queue_pop (&q));
  printf ("queue_pop a: %d\n", *(int*)queue_pop (&q));

  // expect assert
  // queue_pop (&q);
  return 0;
}
#endif // QUEUE_TEST
