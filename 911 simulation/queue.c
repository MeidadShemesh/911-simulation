#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/* Checks if the queue is empty. Returns 1 if empty, 0 otherwise. */
int queue_is_empty(CallQueue* queue)
{
	if (queue->head == NULL)
		return 1;
	return 0;
}

/* Removes and returns the first call in the queue. Returns empty Call if queue is empty. */
Call queue_pop_front(CallQueue* queue) 
{
	if (queue->head!=NULL)
	{
		CallNode* temp = queue->head;
		queue->head = temp->next;
		Call saved = temp->call;
		free(temp);
		queue->size--;
		return saved;
	}

	Call empty = { 0 };
	return empty;
}

/* Inserts a call into the correct position by priority. Equal priority calls are ordered by arrival (FIFO). */
void queue_insert_by_priority(CallQueue* queue, Call call)
{
	CallNode* new_node = (CallNode*)calloc(1, sizeof(CallNode));

	if (new_node == NULL)
	{
		printf("Memory allocation failed\n");
		return;
	}

	new_node->call = call;

	CallNode* curr = queue->head;
	CallNode* prev = NULL; 

	while (curr != NULL && new_node->call.priority >= curr->call.priority)
	{
		prev = curr;
		curr = curr->next;
	}

	if (prev == NULL)
	{
		new_node->next = queue->head;
		queue->head = new_node;
	}

	else
	{
		new_node->next = prev->next;
		prev->next = new_node;
	}

	queue->size++;
}

/* Frees all dynamically allocated nodes in the queue and resets head and size. */
void queue_free(CallQueue* queue)
{
	CallNode* curr = queue->head;

	while (curr != NULL)
	{
		CallNode* temp = curr;
		curr = curr->next;
		free(temp);
	}

	queue->head = NULL;
	queue->size = 0;
}