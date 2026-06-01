#define _CRT_SECURE_NO_WARNINGS
#include "file_io.h"

/* Opens the log file for writing. Returns FILE* on success, NULL on failure */
FILE* open_log_file(const char* filename)
{
	FILE* f = fopen(filename, "w");

	if (f == NULL)
	{
		printf("Error: failed to open file '%s'\n", filename);
		return NULL;
	}

	return f;
}

/* Closes the log file safely. Does nothing if log_file is NULL */
void close_log_file(FILE* log_file)
{
	if (log_file == NULL)
	{
		return;
	}

	fclose(log_file);
}

/* Writes a formatted log line [T=xx.xx] message to both the log file and screen */
void log_event(FILE* log_file, double time, const char* message)
{
	if (log_file == NULL)
	{
		printf("[T= %.2f] %s\n", time, message);
		return;
	}

	fprintf(log_file, "[T= %.2f] %s\n", time, message);
	printf("[T= %.2f] %s\n", time, message);
}

/* Writes a special header line to both the log file and screen without time format */
void log_header(FILE* log_file, const char* header)
{
	if (log_file == NULL)
	{
		printf("%s\n", header);
		return;
	}

	fprintf(log_file, "%s\n",header);
	printf("%s\n", header);
}

/* Saves SimConfig fields to a file in key=value format. Returns 1 on success, 0 on failure */
int save_config(SimConfig* cfg, const char* filename)
{
	FILE* f = fopen(filename, "w");

	if (f == NULL)
	{
		printf("Error: failed to open file '%s'\n", filename);
		return 0;
	}

	fprintf(f, "dispatchers_count=%d\n", cfg->dispatchers_count);
	fprintf(f, "police_count=%d\n", cfg->police_count);
	fprintf(f, "ambulance_count=%d\n", cfg->ambulance_count);
	fprintf(f, "firetruck_count=%d\n", cfg->firetruck_count);
	fprintf(f, "max_sim_time=%f\n", cfg->max_sim_time);
	fprintf(f, "calls_timer=%f\n", cfg->calls_timer);

	fclose(f);

	return 1;
}

/* Loads SimConfig fields from a file and validates all values. Returns 1 on success, 0 on failure */
int load_config(SimConfig* cfg, const char* filename)
{
	FILE* f = fopen(filename, "r");

	if (f == NULL)
	{
		printf("Error: failed to open file '%s'\n", filename);
		return 0;
	}

	fscanf(f, "dispatchers_count=%d\n", &cfg->dispatchers_count);
	fscanf(f, "police_count=%d\n", &cfg->police_count);
	fscanf(f, "ambulance_count=%d\n", &cfg->ambulance_count);
	fscanf(f, "firetruck_count=%d\n", &cfg->firetruck_count);
	fscanf(f, "max_sim_time=%lf\n", &cfg->max_sim_time);
	fscanf(f, "calls_timer=%lf\n", &cfg->calls_timer);

	fclose(f);

	if (cfg->dispatchers_count < 1)
	{
		printf("dispatchers count must be at least 1\n");
		return 0;
	}

	if (cfg->police_count < 1)
	{
		printf("police count must be at least 1\n");
		return 0;
	}

	if (cfg->ambulance_count < 1)
	{
		printf("ambulance count must be at least 1\n");
		return 0;
	}

	if (cfg->firetruck_count < 1)
	{
		printf("firetruck count must be at least 1\n");
		return 0;
	}

	if (cfg->max_sim_time <=0)
	{
		printf("the maximum simulation time must be above 0\n");
		return 0;
	}

	if (cfg->calls_timer <= 0)
	{
		printf("calls timer must be above 0\n");
		return 0;
	}

	return 1;
}
