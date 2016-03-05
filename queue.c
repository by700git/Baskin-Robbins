#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/**
 *  Given data, place it on the queue.  Can be called by multiple threads.
 */
void queue_push(queue_t *queue, void *data) {
/* Your code here */ 
  pthread_mutex_lock(&queue->m);
  if (queue->maxSize>0){
    while (queue->size>=queue->maxSize){
      pthread_cond_wait(&queue->cv,&queue->m);
    }
    queue_node_t* oldhead = queue->head;
    queue_node_t* head = malloc(sizeof(queue_node_t));
    head->data = data;
    head->next = oldhead;
    queue->head = head;
    queue->size+=1;
  }
  else if (queue->maxSize<0){
    queue_node_t* oldhead = queue->head;
    queue_node_t* head = malloc(sizeof(queue_node_t));
    head->data = data;
    head->next = oldhead;
    queue->head = head;
    queue->size+=1;
  }
  pthread_mutex_unlock(&queue->m);
}

/**
 *  Retrieve the data from the front of the queue.  Can be called by multiple
 * threads.
 *  Blocks the queue is empty
 */
void *queue_pull(queue_t *queue) {
  /* Your code here */
  pthread_mutex_lock(&queue->m);
  while (queue->size == 0){
    pthread_cond_wait(&queue->cv,&queue->m);
  }
  void* data = queue->tail->data;
  queue_node_t* before = queue->head;
  while (before->next!=queue->tail){
    before = before->next;
  }
  free(queue->tail);
  queue->tail = before;
  queue->size-=1;
  pthread_mutex_unlock(&queue->m);
  return data;
}

/**
 *  Initializes the queue
 */
void queue_init(queue_t *queue, int maxSize) {
/* Your code here */ 
  queue->maxSize = maxSize;
  queue->size = 0;
  queue->head = NULL;
  queue->tail = NULL;
  pthread_mutex_init(&queue->m,NULL);
  pthread_cond_init(&queue->cv,NULL);
}

/**
 *  Destroys the queue, freeing any remaining nodes in it.
 */
void queue_destroy(queue_t *queue) {
  /* Your code here */ 
  queue_node_t* current = queue->head;
  while (current!=NULL){
    free(current->data);
    current = current->next;
  }
  current = queue->head;
  queue_node_t* buffer = current;
  while (current!=NULL){
    current = current->next;
    free(buffer);
    buffer = current;
  }
  pthread_mutex_destroy(&queue->m);
  free(queue);
}
