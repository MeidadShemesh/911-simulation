#pragma once
#include "defs.h"

FieldUnit** allocate_units_matrix(int rows, int* cols_per_row);
void free_units_matrix(FieldUnit** matrix, int rows);
FieldUnit* find_available_unit(FieldUnit** matrix, int unit_type, int count); 
void update_unit_status(FieldUnit* unit, int new_status);
void init_units_matrix(FieldUnit** matrix, int rows, int* cols_per_row);