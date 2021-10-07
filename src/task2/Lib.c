#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

int addMatrixes(int numberRows1, int numberColumns1, int numberRows2, int numberColumns2, int numberRows3, int numberColumns3, double* firstCellMatrix1, double* firstCellMatrix2, double* firstCellResult)
{
	int currentRow, currentColumn;
	bool flagOver = true;
	long double sum;
	if ((numberRows1 == numberRows2) && (numberColumns1 == numberColumns2))
	{
		if ((numberRows1 == numberRows3) && (numberColumns1 == numberColumns3))
		{
			for (currentRow = 0; currentRow < numberRows1; currentRow++)
			{
				for (currentColumn = 0; currentColumn < numberColumns1; currentColumn++)
				{
					sum = *((firstCellMatrix1 + currentRow * numberColumns1) + currentColumn) + *((firstCellMatrix2 + currentRow * numberColumns1) + currentColumn);
					if ((sum < -1.7E-308) || (sum > 1.7E+308))
					{
						flagOver = false;
					}
					*((firstCellResult + currentRow * numberColumns1) + currentColumn) = *((firstCellMatrix1 + currentRow * numberColumns1) + currentColumn) + *((firstCellMatrix2 + currentRow * numberColumns1) + currentColumn);
				}
			}
			if (flagOver)
			{
				return 0;
			}
			else
			{
				return 2;
			}
		}
		else
		{
			return -1;
		}

	}
	else
	{
		return 1;
	}
}

int multMatrixNum(int numberRows1, int numberColumns1, int numberRows2, int numberColumns2, double* firstCellMatrix, double* firstCellResult, double multiplier)
{
	int currentRow, currentColumn;
	bool flagOver = true;
	long double cell;
	if ((numberRows1 == numberRows2) && (numberColumns1 == numberColumns2))
	{
		for (currentRow = 0; currentRow < numberRows1; currentRow++)
		{
			for (currentColumn = 0; currentColumn < numberColumns1; currentColumn++)
			{
				cell = *((firstCellMatrix + currentRow * numberColumns1) + currentColumn) * multiplier;
				if ((cell < -1.7E-308) || (cell > 1.7E+308))
				{
					flagOver = false;
				}
				*((firstCellResult + currentRow * numberColumns1) + currentColumn) = *((firstCellMatrix + currentRow * numberColumns1) + currentColumn) * multiplier;
			}
		}
		if (flagOver)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		return -1;
	}
}

int multMatrixes(int numberRows1, int numberColumns1, int numberRows2, int numberColumns2, int numberRows3, int numberColumns3, double* firstCellMatrix1, double* firstCellMatrix2, double* firstCellResult)
{
	int currentRow, currentColumn, multiplierIndex = 0;
	bool flagOver = true;
	long double sum;
	if (numberColumns1 == numberRows2)
	{
		if ((numberRows1 == numberRows3) && (numberColumns2 == numberColumns3))
		{
			for (currentRow = 0; currentRow < numberRows1; currentRow++)
			{
				for (currentColumn = 0; currentColumn < numberColumns2; currentColumn++)
				{
					sum = 0;
					for (multiplierIndex = 0; multiplierIndex < numberColumns1; multiplierIndex++)
					{
						sum += *((firstCellMatrix1 + currentRow * numberColumns1) + multiplierIndex) * *((firstCellMatrix2 + multiplierIndex * numberColumns2) + currentColumn);
					}
					if ((sum < -1.7E-308) || (sum > 1.7E+308))
					{
						flagOver = false;
					}
					*((firstCellResult + currentRow * numberColumns3) + currentColumn) = 0;
					for (multiplierIndex = 0; multiplierIndex < numberColumns1; multiplierIndex++)
					{
						*((firstCellResult + currentRow * numberColumns3) + currentColumn) += *((firstCellMatrix1 + currentRow * numberColumns1) + multiplierIndex) * *((firstCellMatrix2 + multiplierIndex * numberColumns2) + currentColumn);
					}
				}
			}
			if (flagOver)
			{
				return 0;
			}
			else
			{
				return 2;
			}
		}
		else
		{
			return -1;
		}

	}
	else
	{
		return 1;
	}
}

void inputMatrix(int numberRows, int numberColumns, double* firstCellMatrix)
{
	int currentRow, currentColumn;

	for (currentRow = 0; currentRow < numberRows; currentRow++)
	{
		for (currentColumn = 0; currentColumn < numberColumns; currentColumn++)
		{
			double character;
			scanf("%lf", &character);
			*((firstCellMatrix + currentRow * numberColumns) + currentColumn) = character;
		}
	}
}

void outputMatrix(int numberRows, int numberColumns, double* firstCellMatrix)
{
	int currentRow, currentColumn;
	for (currentRow = 0; currentRow < numberRows; currentRow++)
	{
		for (currentColumn = 0; currentColumn < numberColumns; currentColumn++)
		{
			printf(" %*f", 10, *((firstCellMatrix + currentRow * numberColumns) + currentColumn));
		}
		printf("\n");
	}
}

void swapRowsDeterm(int numberColumns, int firstRow, int secondRow, double* firstCellMatrix)
{
	double currentRow;
	for (int columnIndex = 0; columnIndex < numberColumns; columnIndex++)
	{
		currentRow = *((firstCellMatrix + firstRow * numberColumns) + columnIndex);
		*((firstCellMatrix + firstRow * numberColumns) + columnIndex) = *((firstCellMatrix + secondRow * numberColumns) + columnIndex);
		*((firstCellMatrix + secondRow * numberColumns) + columnIndex) = currentRow;
	}
}

int checkBeforeMatrixDeterminant(int numberRows, int numberColumns)
{
	int currentRow, currentColumn;
	if (numberRows == numberColumns)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

double getMatrixDeterminant(int numberRows, int numberColumns, double* firstCellMatrix, int* error)
{
	int currentRow, currentColumn;
	double epsilon = 1E-9; // очень маленькое значение. Используется из-за наличия double и float переменных при вычислениях 

	*error = checkBeforeMatrixDeterminant(numberRows, numberColumns);
	if (*error != 0)
	{		
		return 0;
	}
	double determinant = 1;
	for (currentRow = 0; currentRow < numberRows; ++currentRow)
	{
		int transitionIndex = currentRow;
		for (currentColumn = currentRow + 1; currentColumn < numberRows; ++currentColumn)
		{
			if (fabs(*((firstCellMatrix + currentColumn * numberColumns) + currentRow)) > fabs(*((firstCellMatrix + transitionIndex * numberColumns) + currentRow)))
			{
				transitionIndex = currentColumn;
			}
		}
		if (fabs(*((firstCellMatrix + transitionIndex * numberColumns) + currentRow)) < epsilon)
		{
			determinant = 0;
			break;
		}
		swapRowsDeterm(numberColumns, currentRow, transitionIndex, firstCellMatrix);
		if (currentRow != transitionIndex)
		{
			determinant = -1 * determinant;
		}
		determinant *= *((firstCellMatrix + currentRow * numberColumns) + currentRow);
		for (currentColumn = currentRow + 1; currentColumn < numberRows; ++currentColumn)
		{
			*((firstCellMatrix + currentRow * numberColumns) + currentColumn) /= *((firstCellMatrix + currentRow * numberColumns) + currentRow);
		}
		for (currentColumn = 0; currentColumn < numberRows; ++currentColumn)
		{
			if (currentColumn != currentRow && fabs(*((firstCellMatrix + currentColumn * numberColumns) + currentRow)) > epsilon)
			{
				for (transitionIndex = currentRow + 1; transitionIndex < numberRows; ++transitionIndex)
				{
					*((firstCellMatrix + currentColumn * numberColumns) + transitionIndex) -= *((firstCellMatrix + currentRow * numberColumns) + transitionIndex) * *((firstCellMatrix + currentColumn * numberColumns) + currentRow);
				}
			}
		}
	}
	return determinant;
}
