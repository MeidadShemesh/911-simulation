#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "menu.h"

/* Prints the main menu options to the screen */
void print_menu()
{
	printf("==============================\n");
	printf("=   911 DISPATCH SIMULATION  =\n");
	printf("==============================\n");
	printf("1. Run new simulation\n");
	printf("2. Load config from file and run\n");
	printf("3. Save current config to file\n");
	printf("4. View last log\n");
	printf("5. Exit\n");
	printf("==============================\n");
	printf("Enter your choice: ");
}

/* Prompts the user to enter all simulation parameters with input validation */
void get_config_from_user(SimConfig* cfg)
{
	do {
		printf("Enter number of dispatchers: ");
		if (scanf("%d", &cfg->dispatchers_count) != 1)
		{
			while (getchar() != '\n');
			cfg->dispatchers_count = 0;
		}		
		if (cfg->dispatchers_count <= 0)
			printf("Error: value must be above 0\n");
	} while (cfg->dispatchers_count <= 0);

	do {
		printf("Enter number of police units: ");
		if (scanf("%d", &cfg->police_count) != 1)
		{
			while (getchar() != '\n');
			cfg->police_count = 0;
		}
		if (cfg->police_count <= 0)
			printf("Error: value must be above 0\n");
	} while (cfg->police_count <= 0);

	do {
		printf("Enter number of ambulances: ");
		if (scanf("%d", &cfg->ambulance_count) != 1)
		{
			while (getchar() != '\n');
			cfg->ambulance_count = 0;
		}
		if (cfg->ambulance_count <= 0)
			printf("Error: value must be above 0\n");
	} while (cfg->ambulance_count <= 0);

	do {
		printf("Enter number of firetrucks: ");
		if (scanf("%d", &cfg->firetruck_count) != 1)
		{
			while (getchar() != '\n');
			cfg->firetruck_count = 0;
		}
		if (cfg->firetruck_count <= 0)
			printf("Error: value must be above 0\n");
	} while (cfg->firetruck_count <= 0);

	do {
		printf("Enter simulation time (minutes): ");
		if (scanf("%lf", &cfg->max_sim_time) != 1)
		{
			while (getchar() != '\n');
			cfg->max_sim_time = 0;
		}
		if (cfg->max_sim_time <= 0)
			printf("Error: value must be above 0\n");
	} while (cfg->max_sim_time <= 0);

	do {
		printf("Enter average call time (minutes): ");
		if (scanf("%lf", &cfg->calls_timer) != 1)
		{
			while (getchar() != '\n');
			cfg->calls_timer = 0;
		}
		if (cfg->calls_timer <= 0)
			printf("Error: value must be above 0\n");
	} while (cfg->calls_timer <= 0);

	printf("\n");
}

/* Opens and prints the last simulation log file to the screen */
void view_log_file()
{
	FILE* f = fopen(LOG_FILE, "r");

	if (f == NULL)
	{
		printf("No log file found. Run a simulation first.\n");
		return;
	}

	char line[256];
	while (fgets(line, sizeof(line), f) != NULL)
		printf("%s", line);

	fclose(f);
}