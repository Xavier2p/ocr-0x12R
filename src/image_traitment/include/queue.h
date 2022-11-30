#ifndef QUEUE_H
#define QUEUE_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "struct.h"

typedef struct Node_Q
{
    void *value;
    struct Node_Q *next;
} Node_queue;

typedef struct
{
    Node_queue *head;
    Node_queue *tail;
    size_t length;
} MyQueue;

Node_queue *initialize_node_queue(void *value);

void enqueue(MyQueue *queue, void *value);
void *dequeue(MyQueue *queue);
int is_empty(MyQueue *queue);

void free_node_queue(Node_queue *node);
void free_queue(MyQueue *queue);

#endif
