#define _CRT_SECURE_NO_WARNINGS
#pragma once

/* EVENT TYPES */
#define EVENT_CALL_ARRIVAL 0
#define EVENT_DISPATCHER_PICKUP 1
#define EVENT_UNIT_DISPATCHED 2
#define EVENT_CALL_RESOLVED 3

/* PRIORITY TYPES */
#define PRIORITY_NORMAL 3
#define PRIORITY_HIGH 2
#define PRIORITY_CRITICAL 1

/* UNIT TYPES */
#define UNIT_POLICE 0
#define UNIT_AMBULANCE 1
#define UNIT_FIRETRUCK 2

/* DISPATCHER STATUS */
#define DISPATCHER_AVAILABLE 0
#define DISPATCHER_ON_CALL 1

/* UNIT STATUS */
#define UNIT_AVAILABLE 0
#define UNIT_DISPATCHED 1

/* STRINGS AND FILES */
#define MAX_NAME_LEN 50
#define MAX_UNIT_LEN 20
#define LOG_FILE "log.txt"
#define CONFIG_FILE "config.txt"

typedef struct Call
{
	int id;
	int dispatcher_id;
	int unit_type;
	int priority;
	double arrival_time;
	double service_start_time;
	double resolve_time;
}Call;

typedef struct Dispatcher
{
	int id;
	char name[MAX_NAME_LEN];
	int status;
	double total_busy_time;
	int calls_handled;
	double service_start_time;
}Dispatcher;

typedef struct FieldUnit
{
	int type;
	int status;
	int id;
	char unit_code[MAX_UNIT_LEN];
	int total_dispatches;
}FieldUnit;

typedef struct Event
{
	double time;
	int type;
	int call_id;
	int dispatcher_id;
	int unit_id;
	int unit_type;
}Event;

typedef struct FELNode
{
	Event event;
	struct FELNode* next;
}FELNode;

typedef struct FEL
{
	int size;
	struct FELNode* head;
}FEL;

typedef struct CallNode
{
	Call call;
	struct CallNode* next;
}CallNode;

typedef struct CallQueue
{
	CallNode* head;
	int size;
}CallQueue;

typedef struct SimConfig
{
	int dispatchers_count;
	int police_count;
	int ambulance_count;
	int firetruck_count;
	double max_sim_time;
	double calls_timer;
}SimConfig;

typedef struct SimStats
{
	int total_calls;
	int max_queue_length;
	int calls_by_priority[3];
	double total_wait_time;
	double total_resolve_time;
}SimStats;


