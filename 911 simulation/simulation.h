#pragma once
#include "defs.h"
#include <stdio.h>

typedef struct SimState
{
	FEL fel;
	CallQueue waiting_calls;
	Dispatcher* dispatchers;
	FieldUnit** units_matrix;
	int* units_per_type;
	SimStats stats;
	FILE* log_file;
	double current_time;
	int next_call_id;
	Call* active_calls;
	int active_calls_count;
}SimState;

void run_simulation(SimConfig* cfg);
double generate_exponential(double mean);
int generate_priority();
int generate_unit_type();
void init_dispatchers(Dispatcher* arr, int n);
void handle_call_arrival(Event e, SimState* state, SimConfig* cfg);
void handle_dispatcher_pickup(Event e, SimState* state, SimConfig* cfg);
void handle_unit_dispatched(Event e, SimState* state);
void handle_call_resolved(Event e, SimState* state, SimConfig* cfg);
const char* get_priority_name(int priority);
const char* get_unit_name(int unit_type);
