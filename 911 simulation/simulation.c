#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "simulation.h"
#include "stats.h"
#include "units.h"
#include "queue.h"
#include "fel.h"
#include "file_io.h"

/* Runs the full DES simulation: initializes all components, processes events from the FEL, prints the final report, and frees all memory */
void run_simulation(SimConfig* cfg)
{
	srand((unsigned int)time(NULL));

	SimState state = { 0 };

	stats_init(&state.stats);

	state.dispatchers = (Dispatcher*)malloc(cfg->dispatchers_count * sizeof(Dispatcher));
	init_dispatchers(state.dispatchers, cfg->dispatchers_count);

	int cols[] = { cfg->police_count, cfg->ambulance_count, cfg->firetruck_count };
	state.units_per_type = cols;

	state.units_matrix = allocate_units_matrix(3, state.units_per_type);
	init_units_matrix(state.units_matrix, 3, state.units_per_type);
	state.log_file = open_log_file(LOG_FILE);
	state.next_call_id = 1;
	state.active_calls = (Call*)malloc(1000 * sizeof(Call));
	state.active_calls_count = 0;

	Event first = { 0 };
	
	first.time = generate_exponential(cfg->calls_timer);
	first.type = EVENT_CALL_ARRIVAL;

	fel_insert(&state.fel, first);
	log_header(state.log_file, "--- Simulation Started ---");

	while (!fel_is_empty(&state.fel))
	{
		Event e = fel_pop_min(&state.fel);

		if (e.time > cfg->max_sim_time)
			break;

		state.current_time = e.time;

		switch (e.type)
		{
		case EVENT_CALL_ARRIVAL:
			handle_call_arrival(e, &state, cfg);
			break;
		case EVENT_DISPATCHER_PICKUP:
			handle_dispatcher_pickup(e, &state, cfg);
			break;
		case EVENT_UNIT_DISPATCHED:
			handle_unit_dispatched(e, &state);
			break;
		case EVENT_CALL_RESOLVED:
			handle_call_resolved(e, &state, cfg);
			break;
		}
	}

	log_header(state.log_file, "--- Simulation Ended ---\n");
	stats_print_report(&state.stats, cfg, state.dispatchers, cfg->dispatchers_count, state.units_matrix, state.units_per_type);

	close_log_file(state.log_file);

	free(state.dispatchers);
	free_units_matrix(state.units_matrix, 3);
	fel_free(&state.fel);
	queue_free(&state.waiting_calls);
	free(state.active_calls);
}

/* Generates a random time value from an exponential distribution with the given mean */
double generate_exponential(double mean)
{
	double r = (double)rand() / RAND_MAX;

	return -mean * log(1.0 - r);
}

/* Generates a random call priority: 10% critical, 30% high, 60% normal */
int generate_priority()
{
	int r = rand() % 100;

	if (r < 10)
		return PRIORITY_CRITICAL;
	else if (r < 40)
		return PRIORITY_HIGH;
	else
		return PRIORITY_NORMAL;
}

/* Generates a random field unit type: police, ambulance, or firetruck (equal probability) */
int generate_unit_type()
{
	return rand() % 3;
}

/* Initializes an array of n dispatchers with default values */
void init_dispatchers(Dispatcher* arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		arr[i].id = i;
		sprintf(arr[i].name, "D%d", i);
		arr[i].status = DISPATCHER_AVAILABLE;
		arr[i].total_busy_time = 0;
		arr[i].calls_handled = 0;
	}
}

/* Handles a call arrival event: creates a new call, assigns a dispatcher or queues the call, schedules the next arrival */
void handle_call_arrival(Event e, SimState* state, SimConfig* cfg)
{
	Call new_call = { 0 };

	new_call.id = state->next_call_id;
	state->next_call_id++;
	new_call.arrival_time = state->current_time;
	new_call.priority = generate_priority();
	new_call.unit_type = generate_unit_type();
	new_call.dispatcher_id = -1;

	if (state->active_calls_count >= 1000)
	{
		log_event(state->log_file, state->current_time, "Warning: max calls reached, call dropped.");
		return;
	}

	state->active_calls[state->active_calls_count] = new_call;
	state->active_calls_count++;

	stats_update_on_arrival(&state->stats, &new_call, state->waiting_calls.size);

	int found = 0;
	for (int i = 0; i < cfg->dispatchers_count; i++)
	{
		if (state->dispatchers[i].status == DISPATCHER_AVAILABLE)
		{
			Event pickup = { 0 };
			pickup.type = EVENT_DISPATCHER_PICKUP;
			pickup.time = state->current_time;
			pickup.dispatcher_id = state->dispatchers[i].id;
			pickup.call_id = new_call.id;

			fel_insert(&state->fel, pickup);

			found = 1;
			break;
		}
	}

	if (!found)
		queue_insert_by_priority(&state->waiting_calls, new_call);

	Event next_arrival = { 0 };

	next_arrival.type = EVENT_CALL_ARRIVAL;
	next_arrival.time = state->current_time + generate_exponential(cfg->calls_timer);

	fel_insert(&state->fel, next_arrival);

	char msg[100];
	sprintf(msg, "Call C%d arrived. Priority: %s | Type: %s", new_call.id, get_priority_name(new_call.priority), get_unit_name(new_call.unit_type));
	
	log_event(state->log_file, state->current_time, msg);
}

/* Handles a dispatcher pickup event: pops the next call from the queue, updates dispatcher and call status, schedules unit dispatch */
void handle_dispatcher_pickup(Event e, SimState* state, SimConfig* cfg)
{
	Call call = { 0 };

	for (int i = 0; i < cfg->dispatchers_count; i++)
	{
		if (state->dispatchers[i].id == e.dispatcher_id)
		{
			if (e.call_id > 0)
			{
				for (int j = 0; j < state->active_calls_count; j++)
				{
					if (state->active_calls[j].id == e.call_id)
					{
						call = state->active_calls[j];
						break;
					}
				}
			}
			else
			{
				call = queue_pop_front(&state->waiting_calls);
				if (call.id == 0)
					return;
			}

			state->dispatchers[i].status = DISPATCHER_ON_CALL;
			state->dispatchers[i].service_start_time = state->current_time;
			call.service_start_time = state->current_time;
			call.dispatcher_id = e.dispatcher_id;

			for (int j = 0; j < state->active_calls_count; j++)
			{
				if (state->active_calls[j].id == call.id)
				{
					state->active_calls[j].service_start_time = call.service_start_time;
					state->active_calls[j].dispatcher_id = e.dispatcher_id;
					break;
				}
			}

			Event dispatch = { 0 };
			dispatch.type = EVENT_UNIT_DISPATCHED;
			dispatch.time = state->current_time + (rand() % 3 + 1);
			dispatch.call_id = call.id;
			dispatch.dispatcher_id = e.dispatcher_id;
			dispatch.unit_id = call.unit_type;
			fel_insert(&state->fel, dispatch);

			break;
		}
	}

	char msg[100];
	sprintf(msg, "Dispatcher D%d picked up call C%d", e.dispatcher_id, call.id);
	log_event(state->log_file, state->current_time, msg);
}

/* Handles a unit dispatch event: finds an available unit, updates its status, schedules call resolution */
void handle_unit_dispatched(Event e, SimState* state)
{
	FieldUnit* unit = find_available_unit(state->units_matrix, e.unit_id, state->units_per_type[e.unit_id]);
	
	if (unit != NULL)
	{
		update_unit_status(unit, UNIT_DISPATCHED);

		Event resolved = { 0 };
		resolved.type = EVENT_CALL_RESOLVED;
		resolved.time = state->current_time + generate_exponential(10.0);
		resolved.call_id = e.call_id;
		resolved.dispatcher_id = e.dispatcher_id;
		resolved.unit_id = unit->id;
		resolved.unit_type = unit->type;
		fel_insert(&state->fel, resolved);

		char msg[100];
		sprintf(msg, "Unit %s dispatched for call C%d.", unit->unit_code, e.call_id);
		log_event(state->log_file, state->current_time, msg);
	}

	else
	{
		char msg[100];
		sprintf(msg, "No available unit for call C%d. Call remains open.", e.call_id);
		log_event(state->log_file, state->current_time, msg);
	}
}

/* Handles a call resolved event: frees the dispatcher and unit, updates stats, schedules next pickup if queue is not empty */
void handle_call_resolved(Event e, SimState* state, SimConfig* cfg)
{
	for (int i = 0; i < cfg->dispatchers_count; i++)
	{
		if (state->dispatchers[i].id == e.dispatcher_id)
		{
			state->dispatchers[i].status = DISPATCHER_AVAILABLE;
			state->dispatchers[i].total_busy_time += state->current_time - state->dispatchers[i].service_start_time;
			state->dispatchers[i].calls_handled++;
			break;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < state->units_per_type[i]; j++)
		{
			if (state->units_matrix[i][j].id == e.unit_id && state->units_matrix[i][j].type == e.unit_type)
			{
				update_unit_status(&state->units_matrix[i][j], UNIT_AVAILABLE);
				break;
			}
		}
	}

	Call* found_call = NULL;
	for (int i = 0; i < state->active_calls_count; i++)
	{
		if (state->active_calls[i].id == e.call_id)
		{
			found_call = &state->active_calls[i];
			break;
		}
	}

	if (found_call!=NULL)
	{
		found_call->resolve_time = state->current_time;
		stats_update_on_resolve(&state->stats, found_call);
	}

	if (!queue_is_empty(&state->waiting_calls))
	{
		Event pickup = { 0 };
		pickup.type = EVENT_DISPATCHER_PICKUP;
		pickup.time = state->current_time;
		pickup.dispatcher_id = e.dispatcher_id;
		fel_insert(&state->fel, pickup);
	}
	
	char msg[100];
	sprintf(msg, "Call C%d resolved. Dispatcher D%d is now available.", e.call_id, e.dispatcher_id);
	log_event(state->log_file, state->current_time, msg);
}

/* Returns a string representation of a priority level constant (CRITICAL, HIGH, or NORMAL) */
const char* get_priority_name(int priority)
{
	switch (priority)
	{
	case PRIORITY_CRITICAL: return "CRITICAL";
	case PRIORITY_HIGH:     return "HIGH";
	case PRIORITY_NORMAL:   return "NORMAL";
	default:                return "UNKNOWN";
	}
}

/* Returns a string representation of a unit type constant (POLICE, AMBULANCE, or FIRETRUCK) */
const char* get_unit_name(int unit_type)
{
	switch (unit_type)
	{
	case UNIT_POLICE:    return "POLICE";
	case UNIT_AMBULANCE: return "AMBULANCE";
	case UNIT_FIRETRUCK: return "FIRETRUCK";
	default:             return "UNKNOWN";
	}
}