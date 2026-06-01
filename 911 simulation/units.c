#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "units.h"

/* Allocates a dynamic 2D matrix of field units, each row sized by cols_per_row */
FieldUnit** allocate_units_matrix(int rows, int* cols_per_row)
{
	FieldUnit** mat = (FieldUnit**)calloc(rows, sizeof(FieldUnit*));

	if (mat==NULL)
	{
		printf("Memory allocation failed\n");
		return NULL;
	}

	for (int i = 0; i < rows; i++)
	{

		if (cols_per_row[i] == 0)
		{
			mat[i] = NULL;
			continue;
		}

		mat[i] = (FieldUnit*)malloc(cols_per_row[i] * sizeof(FieldUnit));

		if (mat[i] == NULL)
		{
			printf("Memory allocation failed\n");
			free_units_matrix(mat, rows);
			return NULL;
		}
	}

	return mat;
}

/* Frees all rows and the main pointer array of the units matrix */
void free_units_matrix(FieldUnit** matrix, int rows)
{
	for (int i = 0; i < rows; i++)
	{
		free(matrix[i]);
		matrix[i] = NULL;
	}

	free(matrix);
}

/* Searches a row in the matrix for the first available unit, returns pointer or NULL */
FieldUnit* find_available_unit(FieldUnit** matrix, int unit_type, int count)
{
	for (int j = 0; j < count; j++)
	{
		if (matrix[unit_type][j].status == UNIT_AVAILABLE)
		{
			return &matrix[unit_type][j];
		}
	}
	return NULL;
}

/* Updates a unit's status and dispatch time, increments dispatch count if dispatched */
void update_unit_status(FieldUnit* unit, int new_status)
{
	unit->status = new_status;

	if (new_status == UNIT_DISPATCHED)
		unit->total_dispatches++;
}

/* Initializes all units in the matrix with default values and unit code strings */
void init_units_matrix(FieldUnit** matrix, int rows, int* cols_per_row)
{
	const char* prefixes[] = { "POL", "AMB", "FIR" };

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols_per_row[i]; j++)
		{
			matrix[i][j].type = i;
			matrix[i][j].id = j;
			matrix[i][j].status = UNIT_AVAILABLE;
			matrix[i][j].total_dispatches = 0;

			sprintf(matrix[i][j].unit_code, "%s_%d", prefixes[i], j);
		}
	}
}