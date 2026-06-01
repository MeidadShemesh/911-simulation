#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "stats.h"

/* Initializes all SimStats fields to zero. Call once before simulation starts */
void stats_init(SimStats* s)
{
	s->total_calls = 0;
	s->max_queue_length = 0;
	s->calls_by_priority[0] = 0;
	s->calls_by_priority[1] = 0;
	s->calls_by_priority[2] = 0;
	s->total_wait_time = 0;
	s->total_resolve_time = 0;
}

/* Called on each new call arrival. Updates total_calls, calls_by_priority, and max_queue_length */
void stats_update_on_arrival(SimStats* s, Call* c, int current_queue_size)
{
	s->total_calls++;

	int priority_index = c->priority-1;
	s->calls_by_priority[priority_index]++;

	if (current_queue_size > s->max_queue_length)
	{
		s->max_queue_length = current_queue_size;
	}
}

/* Called when a call is resolved. Adds wait time (service_start - arrival) to total_wait_time */
void stats_update_on_resolve(SimStats* s, Call* c)
{
	s->total_wait_time += c->service_start_time - c->arrival_time;
	s->total_resolve_time += c->resolve_time - c->arrival_time;
}

/* Prints full simulation summary report: config, overall performance, priority breakdown, and dispatcher utilization */
void stats_print_report(SimStats* s, SimConfig* cfg, Dispatcher* dispatchers, int num_dispatchers, FieldUnit** units_matrix, int* units_per_type)
{
	printf("==================================\n");
	printf("=       SIMULATION RESULTS       =\n");
	printf("==================================\n");
	printf("--- configuration ---\n");
	printf("Simulation Time:       %.2f minutes\n", cfg->max_sim_time);
	printf("Dispatchers:           %d\n", cfg->dispatchers_count);
	printf("Police units:          %d\n", cfg->police_count);
	printf("Ambulances:            %d\n", cfg->ambulance_count);
	printf("Firetrucks:            %d\n", cfg->firetruck_count);
	printf("Average call time:     %.2f minutes\n\n", cfg->calls_timer);

	printf("--- Overall performance ---\n");
	printf("Total calls received:         %d\n", s->total_calls);
	if (s->total_calls==0)
	{
		printf("Average wait time in queue:   00.00 minutes\n");
		printf("Average handling time:        00.00 minutes\n");
	}
	else
	{
		printf("Average wait time in queue:   %.2f minutes\n", s->total_wait_time / s->total_calls);
		printf("Average handling time:        %.2f minutes\n", s->total_resolve_time / s->total_calls);
	}
	printf("Maximum queue length:         %d\n\n", s->max_queue_length);

	printf("--- Call by priority ---\n");
	if (s->total_calls != 0)
	{
		printf("Critical:   %d  (%.2f%%)\n", s->calls_by_priority[0], (s->calls_by_priority[0] * 100.0) / s->total_calls);
		printf("High:       %d  (%.2f%%)\n", s->calls_by_priority[1], (s->calls_by_priority[1] * 100.0) / s->total_calls);
		printf("Normal:     %d  (%.2f%%)\n\n", s->calls_by_priority[2], (s->calls_by_priority[2] * 100.0) / s->total_calls);
	}
	else
	{
		printf("Critical:   0  (00.00%%)\n");
		printf("High:       0  (00.00%%)\n");
		printf("Normal:     0  (00.00%%)\n\n");
	}

	sort_dispatchers_by_utilization(dispatchers, num_dispatchers);

	printf("--- Dispatcher utilization (sorted) ---\n");
	if (cfg->max_sim_time != 0)
	{
		for (int i = 0; i < num_dispatchers; i++)
		{
			printf("Dispatcher %s: %.2f%% (%d calls)\n", dispatchers[i].name, dispatchers[i].total_busy_time / cfg->max_sim_time * 100.0, dispatchers[i].calls_handled);
		}
	}
	else
	{
		printf("Simulation time is 0, cannot calculate utilization.\n");
	}
	printf("\n");

	printf("--- Field unit dispatches ---\n");
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < units_per_type[i]; j++)
		{
			printf("Unit %s: %d dispatches\n", units_matrix[i][j].unit_code, units_matrix[i][j].total_dispatches);
		}
		printf("\n");
	}

	printf("\n\n");
}

/* Sorts the dispatcher array in ascending order of total_busy_time using bubble sort */
void sort_dispatchers_by_utilization(Dispatcher* arr, int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1 - i; j++)
		{
			if (arr[j].total_busy_time > arr[j + 1].total_busy_time)
			{
				Dispatcher temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}