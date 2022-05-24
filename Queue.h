
#ifndef __QUEUE_H_
#define __QUEUE_H_

void* queue_init_queue();

void queue_enqueue(void* queue, void* data);

void* queue_dequeue(void* queue);

void* queue_poll(void* queue);

void queue_free_queue(void* queue);

void queue_print_queue(void* queue, void (*print_elem)(void*));

int queue_is_empty(void* queue);


#endif __QUEUE_H_
