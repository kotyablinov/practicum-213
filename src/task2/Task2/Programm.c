#define _CRT_SECURE_NO_WARNINGS
#define N 3 // количество строк
#define M 3 // количество столбцов

#define N1 3 // количество строк 1 матрицы
#define M1 3 // количество столбцов 1 матрицы

#define N2 3 // количество строк 2 матрицы
#define M2 3 // количество столбцов 2 матрицы

#define MULTIP 2.5 // множитель

#include "Lib.h"

/// <summary>
/// Пример подсчета значения определителя матрицы.
/// </summary>
void exampleDeterminant()
{
	double matrix1[N][M];
	double determ = 0;
	printf("Matrix\n");
	inputMatrix(N, M, (double*)matrix1);
	determMatrix(N, M, (double*)matrix1, &determ);
	printf("%lf\n", determ);
}

/// <summary>
/// Пример умножения матрицы на число.
/// </summary>
void exampleMultMatrixNum()
{
	double matrix[N][M];
	double resultMatrix[N][M];
	inputMatrix(N, M, (double*)matrix);
	multMatrixNum(N, M, N, M, (double*)matrix, (double*)resultMatrix, MULTIP);
	outputMatrix(N, M, (double*)matrix);
	outputMatrix(N, M, (double*)resultMatrix);
}

/// <summary>
/// Пример проверки работы суммирования матриц.
/// </summary>
void exampleAddMatrix()
{
	double matrix1[N][M];
	double matrix2[N][M];
	double resultMatrix[N][M];
	printf("First matrix\n");
	inputMatrix(N, M, (double*)matrix1);
	printf("Second matrix\n");
	inputMatrix(N, M, (double*)matrix2);
	addMatrixes(N, M, N, M, N, M, (double*)matrix1, (double*)matrix2, (double*)resultMatrix);
	printf("First matrix\n");
	outputMatrix(N, M, (double*)matrix1);
	printf("Second matrix\n");
	outputMatrix(N, M, (double*)matrix2);
	printf("Resulted matrix\n");
	outputMatrix(N, M, (double*)resultMatrix);
}

/// <summary>
/// Пример проверки работы суммирования матриц.
/// </summary>
void exampleMultMatrixes()
{
	double matrix1[N1][M1];
	double matrix2[N2][M2];
	double resultMatrix[N1][M2];
	printf("First matrix\n");
	inputMatrix(N1, M1, (double*)matrix1);
	printf("Second matrix\n");
	inputMatrix(N2, M2, (double*)matrix2);
	multMatrixes(N1, M1, N2, M2, N1, M2, (double*)matrix1, (double*)matrix2, (double*)resultMatrix);
	printf("First matrix\n");
	outputMatrix(N1, M1, (double*)matrix1);
	printf("Second matrix\n");
	outputMatrix(N2, M2, (double*)matrix2);
	printf("Resulted matrix\n");
	outputMatrix(N1, M2, (double*)resultMatrix);
	return 0;
}

int main()
{
	exampleDeterminant();
	return 0;
}