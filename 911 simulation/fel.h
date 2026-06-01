#pragma once
#include "defs.h"

void fel_insert(FEL* fel, Event e);
Event fel_pop_min(FEL* fel);
int fel_is_empty(FEL* fel);
void fel_free(FEL* fel);