#pragma once
#include "defs.h"

int queue_is_empty(CallQueue* queue);
Call queue_pop_front(CallQueue* queue);
void queue_insert_by_priority(CallQueue* queue, Call call);
void queue_free(CallQueue* queue);