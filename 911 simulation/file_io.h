#pragma once
#include "defs.h"
#include <stdio.h>

FILE* open_log_file(const char* filename);
void close_log_file(FILE* log_file);
void log_event(FILE* log_file, double time, const char* message);
void log_header(FILE* log_file, const char* header);
int save_config(SimConfig* cfg, const char* filename);
int load_config(SimConfig* cfg, const char* filename);
