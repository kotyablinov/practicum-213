#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

int addMatrixes(int numberRows1, int numberColumns1, int numberRows2, int numberColumns2, int numberRows3, int numberColumns3, double* firstCellMatrix1, double* firstCellMatrix2, double* firstCellResult)
{
	int i, j;
	bool flagOver = true;
	long double sum;
	if ((numberRows1 == numberRows2) && (numberColumns1 == numberColumns2))
	{
		if ((numberRows1 == numberRows3) && (numberColumns1 == numberColumns3))
		{
			for (i = 0; i < numberRows1; i++)
			{
				for (j = 0; j < numberColumns1; j++)
				{
					sum = *((firstCellMatrix1 + i * numberColumns1) + j) + *((firstCellMatrix2 + i * numberColumns1) + j);
					if ((sum < -1.7E-308) || (sum > 1.7E+308))
					{
						flagOver = false;
					}
					*((firstCellResult + i * numberColumns1) + j) = *((firstCellMatrix1 + i * numberColumns1) + j) + *((firstCellMatrix2 + i * numberColumns1) + j);
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
	int i, j;
	bool flagOver = true;
	long double cell;
	if ((numberRows1 == numberRows2) && (numberColumns1 == numberColumns2))
	{
		for (i = 0; i < numberRows1; i++)
		{
			for (j = 0; j < numberColumns1; j++)
			{
				cell = *((firstCellMatrix + i * numberColumns1) + j) * multiplier;
				if ((cell < -1.7E-308) || (cell > 1.7E+308))
				{
					flagOver = false;
				}
				*((firstCellResult + i * numberColumns1) + j) = *((firstCellMatrix + i * numberColumns1) + j) * multiplier;
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
	int i, j, k = 0;
	bool flagOver = true;
	long double sum;
	if (numberColumns1 == numberRows2)
	{
		if ((numberRows1 == numberRows3) && (numberColumns2 == numberColumns3))
		{
			for (i = 0; i < numberRows1; i++)
			{
				for (j = 0; j < numberColumns2; j++)
				{
					sum = 0;
					for (k = 0; k < numberColumns1; k++)
					{
						sum += *((firstCellMatrix1 + i * numberColumns1) + k) * *((firstCellMatrix2 + k * numberColumns2) + j);
					}
					if ((sum < -1.7E-308) || (sum > 1.7E+308))
					{
						flagOver = false;
					}
					*((firstCellResult + i * numberColumns3) + j) = 0;
					for (k = 0; k < numberColumns1; k++)
					{
						*((firstCellResult + i * numberColumns3) + j) += *((firstCellMatrix1 + i * numberColumns1) + k) * *((firstCellMatrix2 + k * numberColumns2) + j);
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

int inputMatrix(int numberRows, int numberColumns, double* firstCellMatrix)
{
	int i, j;

	for (i = 0; i < numberRows; i++)
	{
		for (j = 0; j < numberColumns; j++)
		{
			double character;
			scanf("%lf", &character);
			*((firstCellMatrix + i * numberColumns) + j) = character;
		}
	}
	return 0;
}

int outputMatrix(int numberRows, int numberColumns, double* firstCellMatrix)
{
	int i, j;
	for (i = 0; i < numberRows; i++)
	{
		for (j = 0; j < numberColumns; j++)
		{
			printf(" %*f", 10, *((firstCellMatrix + i * numberColumns) + j));
		}
		printf("\n");
	}
	return 0;
}

void swapRowsDeterm(int numberColumns, int i, int j, double* firstCellMatrix)
{
	double current;
	int k;
	for (k = 0; k < numberColumns; k++)
	{
		current = *((firstCellMatrix + i * numberColumns) + k);
		*((firstCellMatrix + i * numberColumns) + k) = *((firstCellMatrix + j * numberColumns) + k);
		*((firstCellMatrix + j * numberColumns) + k) = current;
	}
}

int determMatrix(int numberRows, int numberColumns, double* firstCellMatrix, double* determinant)
{
	int i, j;
	double EPS = 1E-9;
	*determinant = 1;
	if (numberRows == numberColumns)
	{
		for (i = 0; i < numberRows; ++i)
		{
			int k = i;
			for (j = i + 1; j < numberRows; ++j)
			{
				if (fabs(*((firstCellMatrix + j * numberColumns) + i)) > fabs(*((firstCellMatrix + k * numberColumns) + i)))
				{
					k = j;
				}
			}
			if (fabs(*((firstCellMatrix + k * numberColumns) + i)) < EPS)
			{
				*determinant = 0;
				break;
			}
			swapRowsDeterm(numberColumns, i, k, firstCellMatrix);
			if (i != k)
			{
				*determinant = -1 * *determinant;
			}
			*determinant *= *((firstCellMatrix + i * numberColumns) + i);
			for (j = i + 1; j < numberRows; ++j)
			{
				*((firstCellMatrix + i * numberColumns) + j) /= *((firstCellMatrix + i * numberColumns) + i);
			}
			for (j = 0; j < numberRows; ++j)
			{
				if (j != i && fabs(*((firstCellMatrix + j * numberColumns) + i)) > EPS)
				{
					for (k = i + 1; k < numberRows; ++k)
					{
						*((firstCellMatrix + j * numberColumns) + k) -= *((firstCellMatrix + i * numberColumns) + k) * *((firstCellMatrix + j * numberColumns) + i);
					}
				}
			}
		}
		return 0;
	}
	else
	{
		return -1;
	}
}
