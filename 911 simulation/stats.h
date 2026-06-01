#pragma once
#include "defs.h"

void stats_init(SimStats* s);
void stats_update_on_arrival(SimStats* s, Call* c, int current_queue_size);
void stats_update_on_resolve(SimStats* s, Call* c);
void stats_print_report(SimStats* s, SimConfig* cfg, Dispatcher* dispatchers, int num_dispatchers, FieldUnit** units_matrix, int* units_per_type);
void sort_dispatchers_by_utilization(Dispatcher* arr, int n);