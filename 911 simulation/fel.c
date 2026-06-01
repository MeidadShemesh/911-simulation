#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "fel.h"

/* Inserts a new event into the FEL in sorted order by time (earliest first) */
void fel_insert(FEL* fel, Event e)
{
	FELNode* new_node = (FELNode*)malloc(sizeof(FELNode));
	
	if (new_node == NULL)
	{
		printf("Memory allocation failed\n");
		return;
	}

	new_node->event = e;

	FELNode* curr = fel->head;
	FELNode* prev = NULL;

	while (curr != NULL && curr->event.time < new_node->event.time)
	{
		prev = curr;
		curr = curr->next;
	}

	if (prev == NULL)
	{
		FELNode* temp = fel->head;
		fel->head = new_node;
		new_node->next = temp;
	}

	else
	{
		prev->next = new_node;
		new_node->next = curr;
	}

	fel->size++;
}

/* Removes and returns the earliest event from the FEL, returns empty event if FEL is empty */
Event fel_pop_min(FEL* fel)
{
	if (fel->head != NULL)
	{
		FELNode* temp = fel->head;
		fel->head = temp->next;
		Event event = temp->event;
		free(temp);
		fel->size--;
		return event;
	}

	Event empty = { 0 };
	return empty;
}

/* Checks if the FEL is empty, returns 1 if empty, 0 otherwise */
int fel_is_empty(FEL* fel)
{
	if (fel->head == NULL)
		return 1;
	return 0;
}

/* Frees all dynamically allocated nodes in the FEL and resets head and size */
void fel_free(FEL* fel)
{
	FELNode* curr = fel->head;

	while (curr != NULL)
	{
		FELNode* temp = curr;
		curr = curr->next;
		free(temp);
	}

	fel->size = 0;
	fel->head = NULL;
}