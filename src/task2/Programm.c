const int ROWS=3; // количество строк
const int COLUMNS=3; // количество столбцов

const int ROWS1=3; // количество строк 1 матрицы
const int COLUMNS1=3; // количество столбцов 1 матрицы

const int ROWS2=3; // количество строк 2 матрицы
const int COLUMNS2=3; // количество столбцов 2 матрицы

const int ROWSRESULT=3; // количество строк результирующей матрицы
const int COLUMNSRESULT=3; // количество столбцов результирующей матрицы

const double MULTIPLIER=2.5; // множитель

#include "Lib.h"

/// <summary>
/// Пример работы функции вычисления определителя матрицы в программе.
/// </summary>
void programExampleDeterminant(double matrix1[ROWS][COLUMNS])
{
	printf("Matrix\n");
	inputMatrix(ROWS, COLUMNS, (double*)matrix1);
	int error = 0;
	double determinant = getMatrixDeterminant(ROWS, COLUMNS, (double*)matrix1, &error);

	if (error == 0)
	{
		printf("%lf\n", determinant);
	}
	else
	{
		printf("Argument exception: sizes of matrix are different.");
	}
}

/// <summary>
/// Пример работы функции умножения матрицы на число в программе.
/// </summary>
void programExampleMultMatrixNumber(double matrix[ROWS1][COLUMNS1], double resultMatrix[ROWS1][COLUMNS2])
{
	inputMatrix(ROWS, COLUMNS, (double*)matrix);
	multMatrixNum(ROWS, COLUMNS, ROWSRESULT, COLUMNSRESULT, (double*)matrix, (double*)resultMatrix, MULTIPLIER);
	outputMatrix(ROWS, COLUMNS, (double*)matrix);
	outputMatrix(ROWSRESULT, COLUMNSRESULT, (double*)resultMatrix);
}

/// <summary>
/// Пример проверки работы функции суммирования матриц в программе.
/// </summary>
void programExampleAddMatrix(double matrix1[ROWS1][COLUMNS1], double matrix2[ROWS2][COLUMNS2], double resultMatrix[ROWS1][COLUMNS2])
{
	printf("First matrix\n");
	inputMatrix(ROWS1, COLUMNS1, (double*)matrix1);
	printf("Second matrix\n");
	inputMatrix(ROWS2, COLUMNS2, (double*)matrix2);
	addMatrixes(ROWS1, COLUMNS1, ROWS2, COLUMNS2, ROWSRESULT, COLUMNSRESULT, (double*)matrix1, (double*)matrix2, (double*)resultMatrix);
	printf("First matrix\n");
	outputMatrix(ROWS1, COLUMNS1, (double*)matrix1);
	printf("Second matrix\n");
	outputMatrix(ROWS2, COLUMNS2, (double*)matrix2);
	printf("Resulted matrix\n");
	outputMatrix(ROWSRESULT, COLUMNSRESULT, (double*)resultMatrix);
}

/// <summary>
/// Пример проверки работы функции суммирования матриц в программе.
/// </summary>
void programExampleMultMatrixes(double matrix1[ROWS1][COLUMNS1],double matrix2[ROWS2][COLUMNS2],double resultMatrix[ROWS1][COLUMNS2])
{
	printf("First matrix\n");
	inputMatrix(ROWS1, COLUMNS1, (double*)matrix1);
	printf("Second matrix\n");
	inputMatrix(ROWS2, COLUMNS2, (double*)matrix2);
	multMatrixes(ROWS1, COLUMNS1, ROWS2, COLUMNS2, ROWS1, COLUMNS2, (double*)matrix1, (double*)matrix2, (double*)resultMatrix);
	printf("First matrix\n");
	outputMatrix(ROWS1, COLUMNS1, (double*)matrix1);
	printf("Second matrix\n");
	outputMatrix(ROWS2, COLUMNS2, (double*)matrix2);
	printf("Resulted matrix\n");
	outputMatrix(ROWS1, COLUMNS2, (double*)resultMatrix);
}

int main()
{
	double matrix[ROWS][COLUMNS];
	double matrix1[ROWS1][COLUMNS1];
	double matrix2[ROWS2][COLUMNS2];
	double resultMatrix[ROWS1][COLUMNS2];
	programExampleDeterminant(matrix1);
	//programExampleMultMatrixes(matrix1, matrix2, resultMatrix);
	//programExampleAddMatrix(matrix1, matrix2, resultMatrix);
	//programExampleMultMatrixNumber(matrix1, resultMatrix);
	return 0;
}
