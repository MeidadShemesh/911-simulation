#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "simulation.h"
#include "file_io.h"
#include "menu.h"


int main()
{
	SimConfig cfg = { 0 };
	int config_loaded = 0;
	int choice = 0;

	while (choice != 5)
	{
		print_menu();
		scanf("%d", &choice);
		while (getchar() != '\n');

		switch (choice)
		{
		case(1):
			get_config_from_user(&cfg);
			config_loaded = 1;
			run_simulation(&cfg);
			break;

		case(2):
			if (load_config(&cfg, CONFIG_FILE))
			{
				config_loaded = 1;
				run_simulation(&cfg);
			}
			else
				printf("Failed to load config. Please enter parameters manually.\n");
			break;

		case(3):
			if (config_loaded == 0)
				printf("No config to save. Please run option 1 or 2 first.\n");
			else
			{
				if (save_config(&cfg, CONFIG_FILE))
				{
					printf("Config saved successfully.\n");
				}
				else
					printf("Failed to save config.\n");
			}
			break;

		case(4):
			view_log_file();
			break;

		case(5):
			printf("Goodbye!\n");
			break;

		default:
			printf("Invalid choice. Please enter a number between 1 and 5.\n");
			break;
		}

	}
	return 0;
}