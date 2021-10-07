/// <summary>
/// Сложение матриц.
/// </summary>
/// <param name="numberRows1"> Количество строк первой матрицы. </param>
/// <param name="numberColumns1"> Количество столбцов первой матрицы. </param>
/// <param name="numberRows2"> Количество строк второй матрицы. </param>
/// <param name="numberColumns2"> Количество столбцов второй матрицы. </param>
/// <param name="numberRows3"> Количество строк результирующей матрицы. </param>
/// <param name="numberColumns3"> Количество столбцов результирующей матрицы. </param>
/// <param name="firstCellMatrix1"> Указатель на первую ячейку первой матрицы. </param>
/// <param name="firstCellMatrix2"> Указатель на первую ячейку второй матрицы. </param>
/// <param name="firstCellResult"> Указатель на первую ячейку результирующей матрицы. </param>
/// <returns> Результат проверки функции на корректность данных. </returns>
int addMatrixes(int numberRows1, int numberColumns1, int numberRows2, int numberColumns2, int numberRows3, int numberColumns3, double* firstCellMatrix1, double* firstCellMatrix2, double* firstCellResult);

/// <summary>
/// Умножение матрицы на вещественное число.
/// </summary>
/// <param name="numberRows1"> Количество строк первой матрицы. </param>
/// <param name="numberColumns1"> Количество столбцов первой матрицы. </param>
/// <param name="numberRows2"> Количество строк второй матрицы. </param>
/// <param name="numberColumns2"> Количество столбцов второй матрицы. </param>
/// <param name="firstCellMatrix"> Указатель на первую ячейку матрицы. </param>
/// <param name="firstCellResult"> Указатель на первую ячейку результирующей матрицы. </param>
/// <param name="multiplier"> Множитель. </param>
/// <returns> Результат проверки функции на корректность данных. </returns>
int multMatrixNum(int numberRows1, int numberColumns1, int numberRows2, int numberColumns2, double* firstCellMatrix, double* firstCellResult, double multiplier);

/// <summary>
/// Умножение матриц.
/// </summary>
/// <param name="numberRows1"> Количество строк первой матрицы. </param>
/// <param name="numberColumns1"> Количество столбцов первой матрицы. </param>
/// <param name="numberRows2"> Количество строк второй матрицы. </param>
/// <param name="numberColumns2"> Количество столбцов второй матрицы. </param>
/// <param name="numberRows3"> Количество строк результирующей матрицы. </param>
/// <param name="numberColumns3"> Количество столбцов результирующей матрицы. </param>
/// <param name="firstCellMatrix1"> Указатель на первую ячейку первой матрицы. </param>
/// <param name="firstCellMatrix2"> Указатель на первую ячейку второй матрицы. </param>
/// <param name="firstCellResult"> Указатель на первую ячейку результирующей матрицы. </param>
/// <returns> Результат проверки функции на корректность данных. </returns>
int multMatrixes(int numberRows1, int numberColumns1, int numberRows2, int numberColumns2, int numberRows3, int numberColumns3, double* firstCellMatrix1, double* firstCellMatrix2, double* firstCellResult);

/// <summary>
/// Вывод матрицы.
/// </summary>
/// <param name="numberRows"> Количество строк матрицы. </param>
/// <param name="numberColumns"> Количество столбцов матрицы. </param>
/// <param name="firstCellMatrix"> Указатель на первую ячейку матрицы. </param>
/// <param name="error"> 
///    <para> Код ошибки (возвращаемое значение): </para> 
///    <para> - если введенная размерность матрицы неверная (матрица должны быть квадратной): -1; </para> 
///    <para> - в иных случаях : 0. </para>
/// </param>
/// <returns> Определитель матрицы. </returns>
double getMatrixDeterminant(int numberRows, int numberColumns, double* firstCellMatrix, int* error);

/// <summary>
/// Ввод матрицы.
/// </summary>
/// <param name="numberRows"> Количество строк матрицы. </param>
/// <param name="numberColumns"> Количество столбцов матрицы. </param>
/// <param name="firstCellMatrix"> Указатель на первую ячейку матрицы (куда записывать введенную матрицу) (возвращаемое значение). </param>
void inputMatrix(int numberRows, int numberColumns, double* firstCellMatrix);

/// <summary>
/// Вывод матрицы.
/// </summary>
/// <param name="numberRows"> Количество строк матрицы. </param>
/// <param name="numberColumns"> Количество столбцов матрицы. </param>
/// <param name="firstCellMatrix"> Указатель на первую ячейку матрицы. </param>
void outputMatrix(int numberRows, int numberColumns, double* firstCellMatrix);
