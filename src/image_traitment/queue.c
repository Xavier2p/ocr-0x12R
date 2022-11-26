#include "../../include/image_traitment/queue.h"
#include <stdio.h>

Node_queue *initialize_node_queue(void *value)
{
    Node_queue *new_node = (Node_queue *)malloc(sizeof(Node_queue));
    if (new_node == NULL)
        errx(1, "Initialize Node_queue: not enough memory");
    new_node->value = value;
    new_node->next = NULL;

    return new_node;
}

void enqueue(MyQueue *queue, void *value)
{
    Node_queue *new_node = initialize_node_queue(value);
    if (is_empty(queue))
    {
        queue->head = new_node;
        queue->tail = new_node;
    }
    else
    {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }
    queue->length++;
}

void *dequeue(MyQueue *queue)
{
    void *res = queue->head->value;
    Node_queue *tmp = queue->head;

    queue->head = queue->head->next;
    free_node_queue(tmp);

    queue->length--;
    return res;
}

int is_empty(MyQueue *queue)
{
    if (queue->head == NULL && queue->tail == NULL && queue->length == 0)
        return 1;
    return 0;
}

void free_node_queue(Node_queue *node)
{
    node->next = NULL;
    free(node);
}

void free_queue(MyQueue *list)
{
    Node_queue *tmp = list->head;
    while (tmp != NULL)
    {
        Node_queue *n = tmp->next;
        free(tmp->value);
        free(tmp);
        tmp = n;
    }
}
